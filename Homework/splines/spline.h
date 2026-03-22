#include"matrix.h"
#include<cassert>

namespace pp {
    int binsearch(const vector& x, double z);
    double linterp(vector x, vector y, double z);
    double linterpInteg(const vector& x, const vector& y, double z);

    struct qspline{
        const int n;
        vector x,y,b,c;
        qspline(const vector& x, const vector& y);
        int binsearch(double z);
        double eval(double z);
        double deriv(double z);
        double integ(double z);
    };

    struct cspline{
        const int n;
        vector x,y,b,c,d;
        cspline(const vector& x, const vector& y);
        int binsearch(double z);
        double eval(double z);
        double deriv(double z);
        double integ(double z);
    };
} //pp