#include<iostream>
#include<cmath>
#include<numbers>
#include"sfuns.h"
#include <iomanip>
constexpr double PI = std::numbers::pi;
constexpr double e = std::numbers::e;
int main(){

double sqrt2 = std::sqrt(2.0);
std:: cout << "sqrt(2) = " << sqrt2 << "\n";

double two_power = std::pow(2.0, 1.0 / 5.0);
std:: cout << "2^1/5 = " << two_power << "\n";

double epi = std::pow(e, PI);
std:: cout << "e^pi = " << epi << "\n";

double pie = std::pow(PI, e);
std:: cout << "pi^e = " << pie << "\n";


for (double x = 1; x <= 10; x += 1){
    std::cout << "gamma(" << x << ") = " << sfuns::fgamma(x) 
    << "\t tgamma(" << x << ") = " << std::tgamma(x) 
    << "\n";
};
std::cout << "\n";

for (double x = -1; x <= 10; x += 1){
    std::cout << "lngamma(" << x << ") = " << sfuns::lngamma(x) 
    << "\t tlngamma(" << x << ") = " << std::lgamma(x)          //new log gamma according to man
    << "\n";
};

return 0;
}
