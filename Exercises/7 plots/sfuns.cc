#include<cmath>
#include<numbers> 
#include"sfuns.h"
#include<limits>
#include<vector>


namespace sfuns{
constexpr double PI = std::numbers::pi;
constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

double fgamma(double x){
	if (x<0) return PI/std::sin(PI*x)/fgamma(1-x);
	if (x<9) return fgamma(x+1)/x;
	double lnfgamma = x*std::log(x+1/(12*x-1/x/10))-x+std::log(2*PI/x)/2;
	return std::exp(lnfgamma);
	}

double lngamma(double x){
    if (x <= 0) return NaN; 
    // if (x < 9) return lngamma(x + 1) - std::log(x);
    return std::log(fgamma(x));
}
double erf(double x){
// single precision error function (Abramowitz and Stegun, from Wikipedia)
if(x<0) return -erf(-x);
std::vector<double> a {0.254829592,-0.284496736,1.421413741,-1.453152027,1.061405429};
double t=1/(1+0.3275911*x);
double sum=t*(a[0]+t*(a[1]+t*(a[2]+t*(a[3]+t*a[4]))));/* the right thing */
return 1-sum*std::exp(-x*x);
} 


}