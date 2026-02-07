#include<iostream>
#include<cmath>
#include<limits>
#include <iomanip>
#include"vec.h"

int main(){

vec a(1,2,5);
vec b;
a.print("a = ");
b.print("b = ");
a.set(4,2,10);
a.print("Set new a = ");

b = a;
b.print("b = a = ");

a *= 2.0;
a.print("a * 2 = ");

a /= 4.0;
a.print("a / 4 = ");


vec apb = a + b;
apb.print("a + b = ");

vec amb = a - b;
amb.print("a - b = ");
std::cout << "\n";

double anorm = a.norm();
std::cout << "Norm of a = " << anorm << "\n";

double adotb = dot(a, b);
std:: cout << "a dot b = " << adotb << "\n";

vec c = cross(a,b);
c.print("a x b = ");

vec d(1, 3, -2);
d.print("d = ");
vec e = cross(a,d);
e.print("e = a x d = ");


bool ae = vec_approx(a,e);
std::cout << "\n" << "a ?= e: " << (ae ? "true":"false") << "\n";       //(ab ? "true":"false") printer bare første eller second option da ab returner 0 (false), 1(true)

a *= 2.0; 
a.print("(a *= 2) =  ");
bool ab = vec_approx(a,b);
std::cout << "a ?= b: " << (ab ? "true":"false") << "\n";
std::cout << "<< overload, a = " << a << "\n\n";

//Tests
vec u(1,2,3); vec v(5,3,1);
u.print("u = "); v.print("v = ");
vec t(u.x + v.x, u.y + v.y, u.z + v.z); t.print("t = ");
(u+v).print("u + v = ");
if(vec_approx(t, (u+v))) std::cout << "test operator+ passed \n\n";

t.~vec(); new (&t) vec(u.x - v.x, u.y - v.y, u.z - v.z); t.print("t = ");
(u-v).print("u - v = ");
if(vec_approx(t, (u-v))) std::cout << "test operator- passed \n\n";

t.~vec(); new (&t) vec(-u.x, -u.y, -u.z); t.print("t = ");
(-u).print("-u = ");
if(vec_approx(t, (-u))) std::cout << "test unary - passed \n\n";

double alpha = 3.5;
t.~vec(); new (&t) vec(alpha * u.x, alpha * u.y, alpha * u.z); t.print("t = ");
(alpha * u).print("alpha * u = ");
if(vec_approx(t, (alpha*u))) std::cout << "test operator* passed \n\n";

t.~vec(); new (&t) vec(u.x / alpha, u.y / alpha, u.z / alpha); t.print("t = ");
(u / alpha).print("u / alpha = ");
if(vec_approx(t, (u/alpha))) std::cout << "test operator/ passed \n\n";

// t.~vec(); new (&t) double(u.x * v.x + u.y * v.y + u.z * v.z); 
double udotv = u.x * v.x + u.y * v.y + u.z * v.z;
std::cout << "t = " << udotv << "\n";
std::cout << "u * v = " << (dot(u,v)) << "\n";
if(approx(udotv, dot(u,v))) std::cout << "dot operator passed \n";


return 0;
}