#include<iostream>
#include<cmath>
#include<limits>
#include <iomanip>
#include"vec.h"
#include<typeinfo>
#include <complex>

int main(){
//Problematisk at trække komplekse ting fra reele, ellers virker det (cast fra complex -> reel er ill-defined - magnitude? realdel?)
// int a = 2;
// double b= 3.7;
// float c = 2.2;
// std::complex d = 3;
std::complex d(3.0,2.3);
std::complex e(3.3,1.2);
std::complex f(2.0,4.8);
// std::complex f(2.0,0.0);

// std::cout << "i = int, d = double, f = float \n";
// std::cout << "a is " << typeid(a).name() << "\n";
// std::cout << "b is " << typeid(b).name() << "\n";
// std::cout << "c is " << typeid(c).name() << "\n";
// std::cout << "d is " << typeid(d).name() << "\n";
// std::cout << "e is " << typeid(e).name() << "\n";
// std::cout << "f is " << typeid(f).name() << "\n";

vec intvec(2,3,8);
vec doublevec(3.2, 1.4, 5.8);
doublevec.print("doublevec = ");
intvec.print("intvec = ");
intvec *= 2.6;
intvec.print("intvec *= 2.6 = ");
intvec /= 2;
intvec.print("intvec /= 2 = ");
// v1 -= 1;
// v1 -= 1.2;

vec v2 (3.1, 8.2, 1.1);
intvec -= v2;
intvec.print("intvec = ");
// vec v2 (a,c,d);
// v1.print("v2(a,c,d) = ");
// double b = intvec.norm();
// std::cout << b << "\n";


vec compv(d,e,f);
// v1 -= compv;
compv -= intvec;
compv.print("compv - v1 = ");

vec newvec = compv + intvec;
newvec.print("newvec compv +v1 = ");

std::cout << "-newvec = " << -newvec << "\n";

std::cout << d.real() << "\n";
std::cout << d.imag() << "\n";


intvec.print("intvec = ");
double b = intvec.norm();
std::cout << b << "\n";

double dotdot = dot(intvec, intvec);
std::cout << "dotdot = " << dotdot << "\n";


double doubleint = dot(intvec, doublevec);
std::cout << "doubleint = " << doubleint << "\n";

// double doublecomp = dot(compv, doublevec);
compv.print("compv = "); doublevec.print("doublevec = ");
std::complex<double> doublecomp = dot(compv, doublevec);
std::cout << "doublecomp = " << doublecomp << "\n";

compv.print("compv= ");
std::cout << "compvec norm = " << compv.norm() << "\n";

vec int_double_cross_vec = cross(intvec, doublevec);

int_double_cross_vec.print("int cross double = ");


double j = 2.0;
// double k = 2.0;
// int m = 2;


// bool ab = vec_approx(intvec, intvec);
std::cout << "\n j = " << j << "\t" << "f = " << f << "\n";
bool ab = approx(j, f);
std::cout << "j ?= f: " << (ab ? "true":"false") << "\n";


// bool intdoublecomp = vec_approx(intvec, doublevec);
bool intdoublecomp = vec_approx(intvec, compv);
// bool intdoublecomp = vec_approx(intvec, intvec);
std::cout << "intvec ?= compv: " << (intdoublecomp ? "true":"false") << "\n \n";




//Tests
vec u(1,2,3); vec v(5.2,3.1,1.6);
// vec v(5,3,1);
u.print("u = "); v.print("v = ");
vec t(u.x + v.x, u.y + v.y, u.z + v.z); t.print("t = ");
(u+v).print("u + v = ");
if(vec_approx(t, (u+v))) std::cout << "test operator+ passed \n\n";

t.~vec(); new (&t) vec(u.x - v.x, u.y - v.y, u.z - v.z); t.print("t = ");
(u-v).print("u - v = ");
if(vec_approx(t, (u-v))) std::cout << "test operator- passed \n\n";

// t.~vec(); new (&t) vec(-u.x, -u.y, -u.z); t.print("t = ");   //den her kan give som bad stuff når t skifter type
t = vec<decltype(t.x)>( -u.x, -u.y, -u.z ); t.print("t = ");
(-u).print("-u = ");
if(vec_approx(t, (-u))) std::cout << "test unary - passed \n\n";

double alpha = 3.5;
// std::complex<double> alpha = 3.5;            //giver problemer med alpha */ u.x udtryk
// t.~vec(); new (&t) vec(alpha * u.x, alpha * u.y, alpha * u.z); t.print("t = ");
vec l(alpha * u.x, alpha * u.y, alpha * u.z); l.print("t = ");
(alpha * u).print("alpha * u = ");
if(vec_approx(l, (alpha*u))) std::cout << "test operator* passed \n\n";

l.~vec(); new (&l) vec(u.x / alpha, u.y / alpha, u.z / alpha); l.print("t = ");
(u / alpha).print("u / alpha = ");
if(vec_approx(l, (u/alpha))) std::cout << "test operator/ passed \n\n";

// t.~vec(); new (&t) double(u.x * v.x + u.y * v.y + u.z * v.z); 
double udotv = u.x * v.x + u.y * v.y + u.z * v.z;
std::cout << "t = " << udotv << "\n";
std::cout << "u * v = " << (dot(u,v)) << "\n";
if(approx(udotv, dot(u,v))) std::cout << "dot operator passed \n";

return 0;
}