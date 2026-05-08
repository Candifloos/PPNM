#pragma once
#include"matrix.h"
#include"qr.h"

namespace pp{
    vector newton(std::function<vector(vector)> f, vector x, double acc = 1e-2, double alpha_min = 1e-3, int max_iter = 100);


}//pp