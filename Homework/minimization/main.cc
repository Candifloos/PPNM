#include"matrix.h"
#include"min.h"


double Rosenbrock(const pp::vector& v){
    //Rosenbrock f(x,y) = (1-x)^2 + 100(y-x^2)^2
    double x = v[0], y = v[1];
    return (1-x)*(1-x) + 100 * (y-x*x)*(y-x*x);
}
double Himmelblau(const pp::vector& v){
    //f(x,y) = (x^2 + y -11)^2 + (x+y^2-7)^2 
    double x = v[0], y = v[1];
    return std::pow(x*x + y -11, 2) + std::pow(x + y*y - 7, 2);  
}

void part_A(){
    pp::vector x0 = {5,-3};
    auto [xr, stepsr] = pp::newton(Rosenbrock, x0);
    auto [xh, stepsh] = pp::newton(Himmelblau, x0);

    std::cout << "Starting point used is (" << x0[0] << "," << x0[1] << ")\n";
    std::cout << "Minimum for Rosenbrock found at " << xr[0] << " " << xr[1] << " with function value " ;
        std::cout << Rosenbrock(xr) << " using " << stepsr << " steps\n";
    std::cout << "Minimum for Himmelblau found at "<< xh[0] << " " << xh[1] << " with function value " ;
        std::cout << Himmelblau(xh) << " using " << stepsh << " steps\n";
    std::cout << "Alligning with expectations that the minimum should be 0 in both cases \n";
}

double Breit_Wigner(double E, pp::vector x){
    double m = x[0], gam = x[1], A = x[2];
    return A / ((E-m)*(E-m) + gam*gam / 4);
}
// double deviation_func(pp::vector x){
//     deviation = 0;
// }

int main(int argc, char** argv){
    bool standard = false;
    
    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--standard") standard = true;
    }

    if (standard){
        part_A();
    }

    pp::vector energy, signal, error;
    double x, y, z;
    while (std::cin >> x >> y >> z){
        energy.push_back(x); signal.push_back(y); error.push_back(z);
    }

    std::function<double(pp::vector)> deviation_func = [energy, signal, error](pp::vector x){
        double deviation = 0, size = energy.size();
        for (int i = 0; i < size; i++){
            deviation += std::pow((Breit_Wigner(energy[i], x) - signal[i]) / error[i], 2);
        };
        return deviation;
    };
    pp::vector init_params = {125, 2, 10};
    auto [fitparams, steps] = newton(deviation_func, init_params);
    std::cout << "Fitparams for Higgs (m, gamma, A) = " << fitparams[0] << " " << fitparams[1] << " " << fitparams[2] << "\n";
    for (double E = 100; E < 160; E += 0.1){
        std::cerr << E << " " << Breit_Wigner(E, fitparams) << "\n";
        // std::cerr << fitparams[0] << " " << fitparams[1] << " " << fitparams[2] << "\n";
    }




    // for (int i = 0; i < energy.size(); i++){
    // std::cout << energy[i] << " " << signal[i] << "\n";
    // }



    
    return 0;
}
