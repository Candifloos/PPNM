#include<cmath>
#include"sfuns.h"
// #include<numbers> 
// #include<limits>


namespace sfuns{
bool approx(double a, double b, double acc, double eps){
	double diff = std::abs(a - b);
    if (diff <= acc) return true;  // absolute tolerance
    double max_ab = std::max(std::abs(a), std::abs(b));
    return diff <= eps * max_ab;   // relative tolerance
}

}