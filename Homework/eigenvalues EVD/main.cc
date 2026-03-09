#include"matrix.h"
#include"evd.h"
#include<utility>
#include <chrono>
#include<fstream>
#include<thread>

pp::matrix eye(int n){
	pp::matrix M(n,n);
	for(int i=0;i<n;i++)M[i,i]=1;
	return M;
	}

struct Task{double rmax; double dr;};
void convergence_run(const Task &t, const std::string &outfile, const std::string &wavefuncoutfile, std::mutex &out_mtx){  //mutex noget med at sørge for at threads ikke interferer med hinanden
    int Npoints = (int)(t.rmax/t.dr - 1);
    pp::vector r(Npoints);
    pp::matrix H(Npoints, Npoints); 
    for (int i = 0; i < Npoints; i++) r[i] = (i+1) * t.dr;
    double diag = 1.0 / t.dr / t.dr, offdiag = -0.5 * diag;
    for (int i = 0; i < Npoints - 1; i++){
        H[i,i]   = diag;
        H[i,i+1] = offdiag;
        H[i+1,i] = offdiag;
    }
    H[Npoints - 1, Npoints - 1] = diag;           //nødt til at tage den her seperat da H[i,i+1] led ellers vil give errors
    for (int i = 0; i < Npoints; i++) H[i,i] += -1/r[i];

    auto [wH,VH] = pp::EVD::cyclic(H);
    VH *= 1/std::pow(t.dr, 0.5);



    std::ofstream myoutput(outfile, std::ios::app);  //opens to append, ellers overwriter den hver gang
    if (myoutput.is_open()){
        std::lock_guard<std::mutex> lock(out_mtx);              //thread safe something something
    myoutput << t.rmax << " " << t.dr << " " << wH[0] << "\n";
    }
    else if (!(outfile == "")) {
    std::cerr << "Error opening files: " << outfile << "\n";
    // return EXIT_FAILURE;     //kan ikke have return i void
    }

    myoutput.close();


    if (!(wavefuncoutfile == "")){
        std::ofstream waveoutput(wavefuncoutfile, std::ios::app);
        if(waveoutput.is_open()){
            std::lock_guard<std::mutex> lock(out_mtx);          //
            for (int i = 0; i < Npoints - 1; i++){
                waveoutput << r[i] << " "<< VH[i,0] << " " << VH[i,1] << " " << VH[i,2] << " " << VH[i,3] << "\n";
            }
        }
        else{
            std::cerr << "Error opening files: " << outfile << "\n";
            // return EXIT_FAILURE;     //kan ikke have return i void
        }
        waveoutput.close();
    }


} //convergence_run
//list of tasks
std::vector<Task> make_rmax_tasks(double start, double end, double step, double dr){
    std::vector<Task> tasks;
    if (step == 0) return tasks;        //
    for (double r = start; r<= end +1e-6; r+=step) tasks.push_back(Task(r,dr));
    return tasks;
}
std::vector<Task> make_dr_tasks(double start, double end, double step, double rmax){
    std::vector<Task> tasks;
    if (step == 0) return tasks;        //
    for (double r = start; r<= end +1e-6; r+=step) tasks.push_back(Task(rmax,r));
    return tasks;
}

int main(int argc, char** argv){
    double rmax = 10, dr = 0.1;
    int nthreads = 1;
    bool EVDtest = true;
    std::string outfile = "";
    std::string wavefuncoutfile = "";
    bool run_rmax_conv = false;
    bool run_dr_conv = false;
    double conv_start = 0.5, conv_step = 0.5, conv_end = 8.0;

    for (int i = 1; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--rmax" && i+1 < argc)          rmax = std::stod(argv[++i]);
        if (arg == "--dr" && i+1 < argc)            dr = std::stod(argv[++i]);
        if (arg == "--threads" && i+1 < argc)       nthreads = std::stoi(argv[++i]);
        if (arg == "--output" && i+1 < argc)        outfile = argv[++i];
        if (arg == "--testoff")                     EVDtest = false; 
        if (arg == "--conv_start" && i+1 < argc)    conv_start = std::stod(argv[++i]);
        if (arg == "--conv_step" && i+1 < argc)     conv_step = std::stod(argv[++i]);
        if (arg == "--conv_end" && i+1 < argc)      conv_end = std::stod(argv[++i]);
        if (arg == "--run_rmax_conv")               run_rmax_conv = true;
        if (arg == "--run_dr_conv")                 run_dr_conv = true;
        if ((arg == "--wavefunc" || arg == "--wavefuncout" || arg == "--wavefuncoutfile" || arg == "-wf") && i < argc) wavefuncoutfile = argv[++i];
    }
    std::cout << run_rmax_conv << " " << conv_end << " " << conv_start << " " << conv_step;
    if (EVDtest){
        int n = 5;
        pp::matrix A(n,n);
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++){
            double rnd = std::rand() % 39;
            A[i,j] = rnd; A[j,i] = rnd;     //yes this overwrites / asigns all twice but whatever, performance impact should be miniscule compared to EVD
        }
        A.print("A = ");
        auto [w,V] = pp::EVD::cyclic(A);
        w.print("w = ");
        V.print("V = ");
        (V.transpose() * A * V).print("V^TAV = ");
        
        pp::matrix D(n,n);
        for (int i = 0; i < n; i++) D[i,i] = w[i];
        D.print("D = ");
        (V * D * V.transpose()).print("VDV^T = ");
        (V * V.transpose()).print("V * V^T = ");
        (V.transpose() * V).print("V^T * V = ");
}
        
    std::vector<Task> tasks;
    if (run_dr_conv) tasks = make_dr_tasks(conv_start, conv_end, conv_step, rmax);
    else if (run_rmax_conv) tasks = make_rmax_tasks(conv_start, conv_end, conv_step, dr);
    else tasks.push_back(Task{rmax, dr});        //if not running convergence on either then just do a single task
    
   if (tasks.empty()){
        std::cerr << "No tasks produced; check sweep parameters.\n";
        return EXIT_FAILURE;
    }

    if (nthreads <= 1){                 //if using only one thread
        std::mutex out_mtx;
        for (const Task &t : tasks) convergence_run(t, outfile, wavefuncoutfile, out_mtx);
        return EXIT_SUCCESS;
    }

    std::atomic<int> next_idx(0);           //atomic noget thread safety https://ryonaldteofilo.medium.com/atomics-in-c-what-is-a-std-atomic-and-what-can-be-made-atomic-part-1-a8923de1384d
    std::mutex out_mtx;
    std::vector<std::thread> threads;
    threads.reserve(nthreads);


    auto worker = [&](int tid){
        while (true){
            int idx = next_idx.fetch_add(1, std::memory_order_relaxed);
            if (idx >= (int)tasks.size()) break;
            // run the task
            convergence_run(tasks[idx], outfile, wavefuncoutfile, out_mtx);
        }
    };

    // spawn threads
    for (int t = 0; t < nthreads; ++t) threads.emplace_back(worker, t);
    // join
    for (auto &th : threads) if (th.joinable()) th.join();



    exit(EXIT_SUCCESS);  


return 0;
}
