#include<iostream>
#include<cmath>
// #include<numbers>
#include"sfuns.h"
#include <iomanip>
#include <string> 

#include <fstream>


// constexpr double PI = std::numbers::pi;
// constexpr double e = std::numbers::e;

int main(int argc,char** argv){
	double xmin=1,xmax=5,dx=0.125;
    bool do_erf = false, do_gamma = false;
	for(int i=0;i<argc;i++){
		std::string arg=argv[i];
		std::cerr<<"i= "<<i<<" arg="<<arg<<"\n";
		if(arg=="-xmin" && i+1<argc)xmin=std::stod(argv[i+1]);
		if(arg=="-xmax" && i+1<argc)xmax=std::stod(argv[i+1]);
		if(arg=="-dx" && i+1<argc)dx=std::stod(argv[i+1]);
        if (arg == "-erf") do_erf = true;
        if (arg == "-gamma") do_gamma = true;
		}
	std::cerr << "xmin= " << xmin << "\n";
	std::cerr<<"xmax= "<<xmax<<"\n";
	std::cerr<<"dx= "<<dx<<"\n";
    
    if (!do_erf && !do_gamma) {
        do_erf = do_gamma = true;
    }
    
    std::cerr << "flags, erf = " << do_erf << " gamma = " << do_gamma << "\n";
    std::cout<<std::scientific;
    for(double x = xmin; x <= xmax; x += dx){
                        std::cout << x;
        if (do_erf)    {std::cout << " " << sfuns::erf(x);}
        if (do_gamma)  {std::cout << " " << sfuns::fgamma(x) << " " << sfuns::lngamma(x);}
                        std::cout << "\n";
    }
    // for(double x = xmin; x <= xmax; x += dx){
    //     std::cout<< x <<" "<< sfuns::erf(x)<< " " << sfuns::fgamma(x) << " " << sfuns::lngamma(x) << "\n";
    // }

	return 0;
}