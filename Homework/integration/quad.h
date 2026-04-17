#pragma once
#include"matrix.h"


namespace pp{
    std::tuple<double, double, int> integrate(std::function<double(double)> f, double a, double b, double acc = 0.001, double eps = 0.001);    //std::nan er en funktion https://en.cppreference.com/w/cpp/numeric/math/nan.html
    std::tuple<double, double, int> CC_integrate(std::function<double(double)> f, double a, double b, double acc = 0.001, double eps = 0.001);    //std::nan er en funktion https://en.cppreference.com/w/cpp/numeric/math/nan.html
    double erf(double z, double acc = 0.001, double eps = 0.001);
} //pp