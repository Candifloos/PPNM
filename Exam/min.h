#pragma once
#include"matrix.h"
#include"qr.h"

namespace pp{
    struct OptimizationResult{ //idea to hold results in seperate struct from Gemini from minimization exercise
        vector x;
        double f_final;
        int iterations = 0;
        int f_evals = 0;
        int qr_evals = 0; 
    }; 

    OptimizationResult newton(std::function<double(vector)> f, vector x, double acc = 1e-3, bool gradient_fallback = false);
    OptimizationResult LMTRDN_minimizer(std::function<double(vector)> f, vector x, double acc = 1e-3, double lambda0 = 0.1);
    OptimizationResult LMTRDN_adaptive_minimizer(std::function<double(vector)> f, vector x, double acc = 1e-3, double lambda0 = 0.1);

}