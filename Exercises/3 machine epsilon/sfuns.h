#pragma once
namespace sfuns{
    bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9);      //default arguments should be given in header according to chatGPT, in .cc is not seen
}