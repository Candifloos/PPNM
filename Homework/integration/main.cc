#include"matrix.h"
#include"quad.h"
#include<fstream>

double inf = std::numeric_limits<double>::infinity();

int main(int argc, char** argv){
    double eps, acc = 0.001;
    bool test = false, erf = false;
    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--eps" && i+1 < argc) eps = std::stod(argv[++i]);
        if (arg == "--acc" && i+1 < argc) acc = std::stod(argv[++i]);
        if (arg == "--test") test = true;
        if (arg == "--erf") erf = true;
    }
    int ncalls = 0;
    auto sqrtx = [](double x){return std::pow(x,0.5);};
    auto isqrtx = [](double x){return std::pow(x,-0.5);};
    auto fx3 = [](double x){return std::pow(1-x*x,0.5);};
    auto fx4 = [&sqrtx](double x){return std::log(x) / sqrtx(x);};
    
    if (test){
    auto [q1, er1, val1] = pp::integrate(sqrtx, 0, 1); auto [q2, er2, val2] = pp::integrate(isqrtx, 0, 1);
    auto always = pp::integrate(sqrtx, 0, 1); auto A = pp::integrate(isqrtx, 0, 1);
    auto [q3, er3, val3] = pp::integrate(fx3, 0, 1); auto [q4, er4, val4] = pp::integrate(fx4, 0, 1);
    std::cout << "Using acc, eps = 0.001 \n";
    std::cout << "Integral of sqrt(x) from 0 to 1 is       " << q1 << " - expected 2/3 \n";
    std::cout << "Integral of 1/sqrt(x) from 0 to 1 is     " << q2 << " - expected 2 \n";
    std::cout << "Integral of sqrt(1-x^2) from 0 to 1 is   " << q3 << " - expected pi/4\n";
    std::cout << "Integral of ln(x)/sqrt(x) from 0 to 1 is " << q4 << " - expected -4\n";
    std::cout << "Calls = " << val1 << " " << val2 << " " << val3 << " " << val4 << "\n";
    
    std::cout << "\n Using Clenshaw-Curtis integrator we get: \n";
    auto [cc2, ccer2, ccval2] = pp::CC_integrate(isqrtx, 0, 1); auto [cc4, ccer4, ccval4] = pp::CC_integrate(fx4, 0, 1);
    std::cout << "Integral of 1/sqrt(x) from 0 to 1 is     " << cc2 << " - expected 2 \n";
    std::cout << "Integral of ln(x)/sqrt(x) from 0 to 1 is " << cc4 << " - expected -4\n";
    std::cout << "Calls = X " << ccval2 << "    X " << ccval4 << "\n";
    std::cout << "That is the non-variable quadrature makes " << val2/ccval2 << ", " << val4/ccval4 << " times the calls \n";
    std::cout << "Scipy uses 231, 315 calls for the two functions respectively \n \n";

    std::cout << "Infinite integrals \n";
    auto if1 = [](double x){return x*x*x * std::exp(-x*x);};
    auto [i1, ierr1, ival1] = pp::integrate(if1, 0, inf);
    auto [i2, ierr2, ival2] = pp::integrate(if1, -inf, 0);
    std::cout << "Integral of x^3*e^(-x^2) from 0 to infinity is " << i1 << " ± " << ierr1 << " using " << ival1 << " calls \n";
    std::cout << "Integral of x^3*e^(-x^2) from -infinity to 0 is " << i2 << " ± " << ierr2 << " using " << ival2 << " calls \n";
    std::cout << "With an expected result of +-1/2 respectively \n";
    auto if2 = [if1](double x){return std::sin(x) * if1(x);};
    auto [i3, ierr3, ival3] = pp::integrate(if2, 0, inf);
    auto [i4, ierr4, ival4] = pp::integrate(if2, -inf, inf);
    std::cout << "Integral of sin(x)*x^3*e^(-x^2) from 0 to infinity is "  << i3 << " ± " << ierr3 << " using " << ival3 << " calls \n";
    std::cout << "Integral of sin(x)*x^3*e^(-x^2) from -infinity to infinity is " << i4 << " ± " << ierr4 << " using " << ival4 << " calls \n";
    std::cout << "With an expected result of 0.862743/2 ≈ 0.43137, 0.862743 respectively \n";
    std::cout << "Scipy uses 75, 75, 75, 150 evaluations for the 4 integrals \n\n";
    
    std::cout << "Ratio between estimated and actual error for above four integrals are: \n";
    std::cout << ierr1 / std::abs(i1 - 0.5) << " " << ierr2 / std::abs(i2 + 0.5) << " " << ierr3 / std::abs(i3 - 0.862743/2) << " " << ierr4 / std::abs(i4 - 0.862743) << "\n";
    std::cout << "i.e. errors generally tend to be overestimated, alligning with expectations \n";
    }

    if (erf){std::cout << acc << " " << std::abs(std::erf(1) - pp::erf(1, acc, eps)) << "\n";}

return 0;
}