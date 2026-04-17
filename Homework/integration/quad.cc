#include"quad.h"
#include<tuple>

namespace pp{
    double pi = std::numbers::pi;

    std::tuple<double, double> integrate_worker(std::function<double(double)> f, double a, double b, double acc, double eps, double f2, double f3, int& ncalls){
        /*Suggestion to make seperate worker function from Gemeni (in order to get ncalls)*/
        ncalls += 1;
        double h = b - a;
        if (f2!=f2){       //when comparing nan-values they are supposedly always false so f2!=f2 is only true for nan https://stackoverflow.com/questions/570669/checking-if-a-double-or-float-is-nan-in-c
            f2 = f(a + 2*h/6);
            f3 = f(a + 4*h/6);
        }
        double f1=f(a+h/6), f4=f(a+5*h/6);
        double Q = (2*f1+f2+f3+2*f4)/6*(b-a); // higher order rule
        double q = (  f1+f2+f3+  f4)/4*(b-a); // lower order rule
        double err = std::abs(Q-q), tol = acc+eps*std::abs(Q); 
        if (err < tol) return std::make_tuple(Q, err);
        else{
            auto [Q1, err1] = integrate_worker(f,a,(a+b)/2, acc/std::pow(2,0.5),eps,f1,f2, ncalls);
            auto [Q2, err2] = integrate_worker(f,(a+b)/2,b, acc/std::pow(2,0.5),eps,f3,f4, ncalls);
            return std::make_tuple(Q1 + Q2, std::sqrt(err1*err1 + err2*err2));
        }

    } //integrate worker

    std::tuple<double, double, int> integrate_inf(std::function<double(double)> f, double a, double b, double acc, double eps){
    /*For handling infinities*/
    if (std::isinf(a) && std::isinf(b)){
        auto func = [a,b,f](double t){return f(t/(1-t*t)) * (1+t*t)/std::pow(1-t*t, 2);};
        return CC_integrate(func, -1, 1, acc, eps);
        };
    if (std::isinf(b)){
        auto func = [a,f](double t){return f(a + t/(1-t)) / std::pow(1-t, 2);};
        return CC_integrate(func, 0, 1, acc, eps);
    };
    if (std::isinf(a)){
        auto func = [b,f](double t){return f(b + t/(1+t)) / std::pow(1+t, 2);};
        return CC_integrate(func, -1, 0, acc, eps);
    };
    return std::make_tuple(std::nan("1"), std::nan("1"), 0);   //shouldnt ever be relevant as the function should only be called if either of (or both) a,b is infinite
    }//integrate_inf

    std::tuple<double, double, int> integrate(std::function<double(double)> f, double a, double b, double acc, double eps){
    int ncalls = 0;
    if (std::isinf(a) || std::isinf(b)) return integrate_inf(f,a,b,acc,eps);

    auto [value, err] = integrate_worker(f,a,b,acc,eps, std::nan("1"), std::nan("1"), ncalls);
    
    return std::make_tuple(value, err, ncalls);
    }//integrate

    std::tuple<double, double, int> CC_integrate(std::function<double(double)> f, double a, double b, double acc, double eps){
        if (std::isinf(a) || std::isinf(b)) return integrate_inf(f,a,b,acc,eps);
        auto func = [a,b,f](double t){
            return f( (a+b)/2 + (b-a)/2*std::cos(t) ) * sin(t) * (b - a) / 2;
        };
        return integrate(func, 0, pi, acc, eps);
    } //cc integrate


    double erf(double z, double acc, double eps){
        double res = 0;
        if (z < 0) {return -erf(-z);};

        if (z >= 0 && z <= 1){                                     
            auto func = [z, acc, eps](double x){return 2/std::pow(pi,0.5) * std::exp(-x*x);};
            res += std::get<0>(integrate(func, 0, std::abs(z), acc, eps));
        }
        else{
            auto func = [z, acc, eps](double x){return 2/std::pow(pi,0.5) * std::exp(- std::pow(z + (1-x)/x, 2)) / (x*x);};
            res += 1 - std::get<0>(integrate(func, 0, 1, acc, eps));
        }
        return res;
    }//erf

} //pp
