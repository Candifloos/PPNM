#include<iostream>
#include<cmath>
#include<limits>
#include <iomanip>
#include"sfuns.h"

int main(){
// """Part 1""";

float       f = 1.0f; while((float)      (1.0f+f) != 1.0f){f/=2.0f;} f*=2.0f;   //Deler bare med 2 indtil 1+f=1
double      d = 1.0;  while((double)     (1.0+d)  != 1.0) {d/=2.0;}  d*=2.0 ;   //f,L siger bare at det skal være float/Long, kan også smide d på double men er standard så unødvendigt
long double l = 1.0L; while((long double)(1.0L+l) != 1.0L){l/=2.0L;} l*=2.0L;   //(double) caster resultatet tilbage til en double, er unødvendig da operationerne gøres med de andre types
std::printf("      float eps = %g\t",f); std::cout << "System epsilon = " << std::numeric_limits<float>::epsilon() << "\t" << "2^(-23) = " << std::pow(2,-23) << "\n";
std::printf("     double eps = %g\t",d); std::cout << "System epsilon = " << std::numeric_limits<double>::epsilon() << "\t" << "2^(-52) = " << std::pow(2,-52) << "\n";
std::printf("long double eps = %Lg\t",l); std::cout << "System epsilon = " << std::numeric_limits<long double>::epsilon() << "\n\n";

// """Part 2""";
double tiny = d / 2;
double a = 1 + tiny + tiny;
double b = tiny + tiny + 1;

std::cout << std::fixed << std::setprecision(17);
std::cout << "           tiny = " << tiny << "\n";
std::cout << "a = 1+tiny+tiny = " << a << "\n";             //1+tiny +tiny = 1 (til rounding) +tiny = 1
std::cout << "b = tiny+tiny+1 = " << b << "\n";             //tiny+tiny bliver eps + 1 = 1 + eps

std::cout << "a==b? " << (a==b ? "true":"false") << "\n";
std::cout << "a>1 ? " << (a>1  ? "true":"false") << "\n";
std::cout << "b>1 ? " << (b>1  ? "true":"false") << "\n\n";

// """Part 3""";
double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1; double d2 = 8*0.1;

std::cout << "d1 == d2? " << (d1==d2 ? "true":"false") << "\n"; 
std::cout << std::fixed << std::setprecision(17);
std::cout << "d1 = " << d1 << "\n";
std::cout << "d2 = " << d2 << "\n";

//Part 4
std::cout << "Our implementation d1 == d2? " << (sfuns::approx(d1,d2) ? "true":"false") << "\n\n"; 

double d3 = 11.0; double d4 = 11.0 + 1e-8;
std::cout << "d3 = " << d3 << "\n";
std::cout << "d4 = " << d4 << "\n";
std::cout << "Our implementation d3 == d4? " << (sfuns::approx(d3,d4) ? "true":"false") << "\n"; 


return 0;
}