#include<cmath>
#include"vec.h"
#include <string>
#include<iostream>



// double dot(const vec& a, const vec& b){
//     return a.x * b.x + a.y * b.y + a.z * b.z;
// }

// vec cross(const vec& a, const vec& b){
//     double newx = a.y * b.z - a.z * b.y;
//     double newy = a.z * b.x - a.x * b.z;
//     double newz = a.x * b.y - a.y * b.x;
//     return vec(newx, newy, newz);
// }


// vec& vec::operator+=(const vec& v){
//     x += v.x; y += v.y; z += v.z;
//     return *this;
// }
// vec& vec::operator-=(const vec& v){
//     x -= v.x; y -= v.y; z -= v.z;
//     return *this;


// vec operator-(const vec& v){;
//     double x = -v.x; double y = -v.y; double z = -v.z;
//     return vec(x,y,z);
// }
// vec operator-(const vec& a, const vec& b){
//     vec v = a;
//     v -= b;
//     return v;
// }
// vec operator+(const vec& a, const vec& b){
//     vec v = a;                              //kunne også gøres med vec v(a) nok pænere da den bruger basics ig
//     v += b;                                 //Kunne også passe a som kopi (så ikke constant og & bare vec a) og sige +=b og returne direkte - optimerer en reference passing & mere moderne
//     return v;
// }
// vec operator*(const vec& a, double d){
//     vec v = a;
//     v *= d;
//     return v;
// }
// vec operator*(double d, const vec& a){
//     vec v = a;
//     v *= d;
//     return v;
// }
// vec operator/(const vec& a, double d){
//     vec v = a;
//     v /= d;
//     return v;
// }







// bool approx(double a, double b, double acc, double eps){
// 	double diff = std::abs(a - b);
//     if (diff <= acc) return true;  // absolute tolerance
//     double max_ab = std::max(std::abs(a), std::abs(b));
//     return diff <= eps * max_ab;   // relative tolerance
// }

// // approximate equality
// bool vec_approx(const vec& a, const vec& b, double acc, double eps){
// 	if(!approx(a.x,b.x, acc, eps))return false;
// 	if(!approx(a.y,b.y, acc, eps))return false;
// 	if(!approx(a.z,b.z, acc, eps))return false;
// 	return true;
// ;
// }

// std::ostream& operator<<(std::ostream& os, const vec& v){
//     os << "{ " << v.x << ", " << v.y << ", " << v.z << " } ";
//     return os;
//     }
