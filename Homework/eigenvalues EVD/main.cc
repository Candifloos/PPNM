#include"matrix.h"
#include"evd.h"
#include<utility>
#include <chrono>
#include<fstream>

pp::matrix eye(int n){
	pp::matrix M(n,n);
	for(int i=0;i<n;i++)M[i,i]=1;
	return M;
	}

int main(int argc, char** argv){
// int main(){    
    double rmax = 10, dr = 0.1;
    bool EVDtest = true;
    std::string outfile = "";
    std::string wavefuncoutfile = "";

    for (int i = 1; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--rmax" && i < argc) rmax = std::stod(argv[++i]);
        if (arg == "--dr" && i < argc) dr = std::stod(argv[++i]);
        if (arg == "--output" && i < argc) outfile = argv[++i];
        if (arg == "--testoff") EVDtest = false; 
        if ((arg == "--wavefunc" || arg == "--wavefuncout" || arg == "--wavefuncoutfile" || arg == "-wf") && i < argc) wavefuncoutfile = argv[++i];
    }

    if (EVDtest){
	int n = 5;
    pp::matrix A(n,n);
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++){
        double rnd = std::rand() % 39;
        A[i,j] = rnd; A[j,i] = rnd;     //yes this overwrites / asigns all twice but whatever
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
    

    int Npoints = (int)(rmax/dr - 1);
    pp::vector r(Npoints);
    pp::matrix H(Npoints, Npoints); 
    for (int i = 0; i < Npoints; i++) r[i] = (i+1) * dr;
    double diag = 1.0 / dr / dr, offdiag = -0.5 * diag;
    for (int i = 0; i < Npoints - 1; i++){
        H[i,i]   = diag;
        H[i,i+1] = offdiag;
        H[i+1,i] = offdiag;
    }
    H[Npoints - 1, Npoints - 1] = diag;           //nødt til at tage den her seperat da H[i,i+1] led ellers vil give errors
    for (int i = 0; i < Npoints; i++) H[i,i] += -1/r[i];
    // r.print("r = ");
    // H.print("H = ");

    auto [wH,VH] = pp::EVD::cyclic(H);
    VH *= 1/std::pow(dr, 0.5);
    // wH.print("wH = ");
    // VH[0].print("vH = ");

    std::ofstream myoutput(outfile, std::ios::app);  //opens to append, ellers overwriter den hver gang
    if (myoutput.is_open()){
    myoutput << rmax << " " << dr << " " << wH[0] << "\n";
    }
    else if (!(outfile == "")) {
    std::cerr << "Error opening files: " << outfile << "\n";
    return EXIT_FAILURE;
    }

    myoutput.close();

    if (!(wavefuncoutfile == "")){
        std::ofstream waveoutput(wavefuncoutfile);
        if(waveoutput.is_open()){
            for (int i = 0; i < Npoints - 1; i++){
                waveoutput << r[i] << " "<< VH[i,0] << " " << VH[i,1] << " " << VH[i,2] << " " << VH[i,3] << "\n";
            }
        }
        else{
            std::cerr << "Error opening files: " << outfile << "\n";
            return EXIT_FAILURE;
        }
        waveoutput.close();
    }


    exit(EXIT_SUCCESS);  


    //PLOT BØLGEFUNKTIONERNE

    // auto t0 = std::chrono::high_resolution_clock::now();
    // auto [QA,RA] = pp::QR::decomp(A);
    // auto t1 = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed_time = t1 - t0;

    // std::ofstream myoutput(outfile, std::ios::app);         //opens to append, ellers overwriter den hver gang
    // if (myoutput.is_open()){
    // myoutput << Msize << " " << elapsed_time << "\n";
    // }
    // else if (!(outfile == "")) {
    // std::cerr << "Error opening files: " << outfile << "\n";
    // return EXIT_FAILURE;
    // }

    // myoutput.close();
    // exit(EXIT_SUCCESS);


return 0;
}