#include <iostream>
#include <cmath>
#include <iomanip>
#include "ann.h"
#include "matrix.h"


double g(double x) {
    return std::cos(5.0 * x - 1.0) * std::exp(-x * x);
}
double g_prime(double x) {
    //dg/dx = -5sin(5x-1)e^(-x^2) -2xcos(5x-1)e^(-x^2)
    return (-5.0 * std::sin(5.0 * x - 1.0) - 2.0 * x * std::cos(5.0 * x - 1.0)) * std::exp(-x * x);
}
double g_doubleprime(double x){
    //d^2g/dx^2 = -25cos(5x-1)e^(-x^2) + 10xsin(5x-1)e^(-x^2) -2cos(5x-1)e^(-x^2) + 10xsin(5x-1)e^(-x^2) + 4x^2cos(5x-1)e^(-x^2)
    // = (-27cos(5x-1) + 20xsin(5x-1)  + 4x^2cos(5x-1))e^(-x^2)
    return (-25*std::cos(5*x-1) + 20*x*std::sin(5*x-1) + 4*x*x*std::cos(5*x-1)) * std::exp(-x * x);
}
double g_antideriv(double x) {
    // Gemini trapezoid integration
    int steps = 1000;
    double sum = 0.0;
    double step_size = x / steps;
    for (int i = 0; i < steps; ++i) {
        double x1 = i * step_size;
        double x2 = (i + 1) * step_size;
        sum += 0.5 * (g(x1) + g(x2)) * step_size;
    }
    return sum;
}

double gaussian_wavelet(double x) {
    return x * std::exp(-x * x);
}



int main() {
    
    int N = 25; 
    pp::vector x_train(N);
    pp::vector y_train(N);
    
    for (int i = 0; i < N; i++) {
        x_train[i] = -1.0 + 2.0 * i / (N - 1);
        y_train[i] = g(x_train[i]);
    }

    int hidden_neurons = 3;
    pp::ann network(hidden_neurons, gaussian_wavelet);

    network.train(x_train, y_train);

    // int N_test = 100;
    // for (int i = 0; i < N_test; i++){
    //     double x_test = -1.0 + 2.0 * i / (N_test - 1);
    //     double true_val = g(x_test);
    //     double pred_val = network.response(x_test);
    //     std::cout << x_test << " " << true_val << " " << pred_val << "\n";
    // }

    double ann_integral_zero = network.antiderivative(0.0);

    int N_test = 100;
    for (int i = 0; i < N_test; i++) {
        double x_test = -1.0 + 2.0 * i / (N_test - 1);
        
        // Output format: 
        // 1:x  2:g(x)  3:ann(x)  4:g'(x)  5:ann'(x)  6:g''(x)  7:ann''(x)  8:g_int(x)  9:ann_int(x)
        std::cout << x_test << " " 
                  << g(x_test) << " " << network.response(x_test) << " "
                  << g_prime(x_test) << " " << network.derivative(x_test) << " "
                  << g_doubleprime(x_test) << " " << network.derivative2(x_test) << " "
                  << g_antideriv(x_test) << " " << (network.antiderivative(x_test) - ann_integral_zero) << "\n";
    }

    return 0;
}




// #include"matrix.h"
// #include"min.h"


// double Rosenbrock(const pp::vector& v){
//     //Rosenbrock f(x,y) = (1-x)^2 + 100(y-x^2)^2
//     double x = v[0], y = v[1];
//     return (1-x)*(1-x) + 100 * (y-x*x)*(y-x*x);
// }
// double Himmelblau(const pp::vector& v){
//     //f(x,y) = (x^2 + y -11)^2 + (x+y^2-7)^2 
//     double x = v[0], y = v[1];
//     return std::pow(x*x + y -11, 2) + std::pow(x + y*y - 7, 2);  
// }

// void part_A(){
//     pp::vector x0 = {5,-3};
//     auto [xr, stepsr] = pp::newton(Rosenbrock, x0);
//     auto [xh, stepsh] = pp::newton(Himmelblau, x0);

//     std::cout << "Starting point used is (" << x0[0] << "," << x0[1] << ")\n";
//     std::cout << "Minimum for Rosenbrock found at " << xr[0] << " " << xr[1] << " with function value " ;
//         std::cout << Rosenbrock(xr) << " using " << stepsr << " steps\n";
//     std::cout << "Minimum for Himmelblau found at "<< xh[0] << " " << xh[1] << " with function value " ;
//         std::cout << Himmelblau(xh) << " using " << stepsh << " steps\n";
//     std::cout << "Alligning with expectations that the minimum should be 0 in both cases \n";
// }

// double Breit_Wigner(double E, pp::vector x){
//     double m = x[0], gam = x[1], A = x[2];
//     return A / ((E-m)*(E-m) + gam*gam / 4);
// }
// // double deviation_func(pp::vector x){
// //     deviation = 0;
// // }

// int main(int argc, char** argv){
//     bool standard = false;
    
//     for (int i = 0; i < argc; i++){
//         std::string arg = argv[i];
//         if (arg == "--standard") standard = true;
//     }

//     if (standard){
//         part_A();
//     }

//     pp::vector energy, signal, error;
//     double x, y, z;
//     while (std::cin >> x >> y >> z){
//         energy.push_back(x); signal.push_back(y); error.push_back(z);
//     }

//     std::function<double(pp::vector)> deviation_func = [energy, signal, error](pp::vector x){
//         double deviation = 0, size = energy.size();
//         for (int i = 0; i < size; i++){
//             deviation += std::pow((Breit_Wigner(energy[i], x) - signal[i]) / error[i], 2);
//         };
//         return deviation;
//     };
//     pp::vector init_params = {125, 2, 10};
//     auto [fitparams, steps] = newton(deviation_func, init_params);
//     std::cout << "Fitparams for Higgs (m, gamma, A) = " << fitparams[0] << " " << fitparams[1] << " " << fitparams[2] << "\n";
//     for (double E = 100; E < 160; E += 0.1){
//         std::cerr << E << " " << Breit_Wigner(E, fitparams) << "\n";
//         // std::cerr << fitparams[0] << " " << fitparams[1] << " " << fitparams[2] << "\n";
//     }




//     // for (int i = 0; i < energy.size(); i++){
//     // std::cout << energy[i] << " " << signal[i] << "\n";
//     // }



    
//     return 0;
// }
