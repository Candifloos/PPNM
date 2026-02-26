#include"matrix.h"
#include<utility>
#include <chrono>
#include<fstream>

pp::matrix eye(int n){
	pp::matrix M(n,n);
	for(int i=0;i<n;i++)M[i,i]=1;
	return M;
	}

int main(int argc, char** argv){
    int m = 5;
	int n = 3;
    

	pp::matrix M(m,n);
	for(int i=0;i<M.size1();i++)
	for(int j=0;j<M.size2();j++)
    M[i,j]= std::rand() % 11;
	M.print("M=");

    //Decomp to Q & R
    auto [Q,R] = pp::QR::decomp(M);
    Q.print("Q = ");
    R.print("R = ");
    (Q*R).print("Q * R = ");
    (Q.transpose() * Q).print("Q^T * Q =");
    //Find determinant
    double detR = pp::QR::det(R);
    std::cout << "\n |det(M)| = |det(R)| = " << detR << "\n \n";
    
    //Solve for x
    pp::matrix M2(n,n);
	for(int i=0;i<M2.size1();i++)
	for(int j=0;j<M2.size2();j++)
    M2[i,j]= std::rand() % 11;
    M2.print("M2=");
    auto [Q2,R2] = pp::QR::decomp(M2);
    Q2.print("Q2 = ");
    R2.print("R2 = ");
    (Q2*R2).print("Q2 * R2 = ");
    (Q2.transpose() * Q2).print("Q2^T * Q2 =");
    pp::vector b(n);
    for (int i = 0; i < b.size(); i++) b[i] = std::rand() % 39;
    b.print("b = ");
    pp::vector x = pp::QR::solve(Q2,R2,b);
    x.print("x = ");
    (M2 * x).print("M2 * x = ");
    pp::matrix inverseM2 = pp::QR::inverse(Q2, R2);
    inverseM2.print("M2^-1 = ");
    (M2 * inverseM2).print("M2 * M2^-1 = ");


    //Timing stuff
    int Msize = 100;
    std::string outfile = "";           //laver custom output stream til times som i exercise 5
    for (int i = 1; i < argc; i++){         
        std::string s = argv[i];
        if (s == "--size" && i + 1 < argc)   Msize = std::stoi(argv[++i]);
        if (s == "--output" && i + 1 < argc) outfile = argv[++i];
        // if (s == "--input" && i + 1 < argc)  infile = argv[++i];
    }
    pp::matrix A(Msize,Msize);
    auto t0 = std::chrono::high_resolution_clock::now();

    auto [QA,RA] = pp::QR::decomp(A);

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = t1 - t0;

    std::ofstream myoutput(outfile, std::ios::app);         //opens to append, ellers overwriter den hver gang
    if (myoutput.is_open()){
    myoutput << Msize << " " << elapsed_time << "\n";
    }
    else if (!(outfile == "")) {
    std::cerr << "Error opening files: " << outfile << "\n";
    return EXIT_FAILURE;
    }

    myoutput.close();
    exit(EXIT_SUCCESS);


return 0;
}