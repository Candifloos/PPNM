#include"matrix.h"
#include"qr.h"
#include"ols.h"
#include<fstream>


int main(){
    pp::vector ts{1,2,3,4,6,9,10,13,15};
    pp::vector as{117, 100, 88, 72, 53, 29.5, 25.2, 15.2, 11.1};
    pp::vector das{6, 5, 4, 4, 4, 3, 3, 2, 2};

    std::vector<std::function<double(double)>> fs {
	[](double z) { return 1.0; },
	[](double z) { return -z; },
	};
    pp::vector logas(as.size()); pp::vector logdas(as.size());
    for (int i = 0; i < as.size(); i++) logas[i] = std::log(as[i]);
    for (int i = 0; i < as.size(); i++) logdas[i] = das[i] / as[i];   //f(y) = ln(y) => df = f'(y)dy  = 1/y dy
    
    auto [cs, cov] = pp::lsfit(fs, ts, logas, logdas);
    double da = std::sqrt(cov[0,0]), dlambda = std::sqrt(cov[1,1]);
    
    std::ofstream outData("data.txt");
    for (int i = 0; i < ts.size(); i++){
        outData << ts[i] << " " << logas[i] << " " << logdas[i] << "\n";
    }
    outData.close();
    
    std::ofstream outFit("fitparams.txt");
    for (int i = 0; i < cs.size(); i++){
        outFit << cs[i] << " " << std::sqrt(cov[i,i]) << "\n";
    }
    outFit.close();
    
    //y(t) = ae^-kt
    // ln(y) = ln(ae^-kt) = ln(a) + (-kt) = ln(a) - kt
    double T_half = std::log(2) / cs[1];
    double dT_half = std::log(2) / (cs[1] * cs[1]) * dlambda;
    double stddevs = (T_half - 3.6319) / dT_half;
    std::cout << "As f(y) = ln(y) the uncertainty dy can be found as df = f'(y)dy  = 1/y dy \n \n";
    std::cout << "Optimal parameters found to be a = " << cs[0] << ",  lambda = " << cs[1] << "\n";
    cov.print("With a covariance matrix of ");
    std::cout << "Giving errors as the square root of the diagonal = " << da << ", " << dlambda << "\n";
    std::cout << "\n Determined half life from fit = " << T_half << " +- " << dT_half << " days, modern value is 3.6319 days \n";
    std::cout << "Where the error is found from dT = |T'(lambda) dlambda| = ln(2)/lambda^2 dlambda \n";
    std::cout << "The found value is thus " << stddevs << " standard deviations from the modern value \n";

return 0;
}