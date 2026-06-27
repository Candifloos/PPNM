#pragma once
#include"matrix.h"
#include"qr.h"

namespace pp{
    struct OptimizationResult{ //idea to hold results in seperate struct from Gemini
        vector x;
        int steps = 0;
    }; 

    // vector newton(std::function<double(vector)> f, vector x, double acc = 1e-3);
    OptimizationResult newton(std::function<double(vector)> f, vector x, double acc = 1e-3);

}