#include"spline.h"
// #include<cassert>

namespace pp {
    int binsearch(const vector& x, double z)
        {/* locates the interval for z by bisection */ 
        assert( z>=x[0] && z<=x[x.size()-1] );
        int i=0, j=x.size()-1;
        while(j-i>1){
            int mid=(i+j)/2;
            if(z>x[mid]) i=mid; else j=mid;
            }
        return i;
        }

    double linterp(vector x, vector y, double z){
        int i=binsearch(x,z);
        double dx=x[i+1]-x[i];
        assert(dx>0);
        double dy=y[i+1]-y[i];
        return y[i]+dy/dx*(z-x[i]);
        }

    double linterpInteg(const vector& x, const vector& y, double z){
        int zidx = binsearch(x,z);
        double sum = 0;
        for (int i = 0; i < zidx; i++){
            double dy = y[i+1] - y[i]; double dx = x[i+1] - x[i];
            sum += y[i] * dx + dy/dx * dx*dx/2;                              //integralet for det i'te interval y[i]+dy/dx*(z-x[i])
        }
        double yz = linterp(x,y,z);
        double dy = yz - y[zidx]; double dx = z - x[zidx];
        sum += y[zidx] * dx + dy/dx * dx*dx/2;
        return sum;
    }
    
    qspline::qspline(const vector& x_in, const vector& y_in)
        : n(x_in.size()), x(x_in), y(y_in), b(n - 1), c(n - 1)
        {
            double oldp = 0;    //need p_(i-1) to calculate c[i]
            for (int i = 0; i < n - 1; ++i) {
                double dy = y[i+1] - y[i]; double dx = x[i+1] - x[i];
                double newp = dy/dx;
                if (i == 0) c[i] = 0;
                else c[i] = 1/dx * (newp - oldp - c[i-1] * (x[i] - x[i-1]));
                b[i] = newp - c[i]*dx;
                oldp = newp;
            }
        };
    int qspline::binsearch(double z){
        return pp::binsearch(this->x, z);       //kan vist også nøjes med bare at skrive x, syntax for some reason this->x og ikke this.x (noget med pointers i think)
    }                                           //Skal have pp:: da den ellers søger i qspline namespaceog finder sig selv ig
    double qspline::eval(double z){
        int i=binsearch(z);                     //ingen pp her, brug intern funktion
        double dx = z - x[i];
        return y[i] + b[i] * dx + c[i] * dx*dx;     //kunne have this-> på alle y,b,x,,c i her i suppose
    }

    double qspline::deriv(double z){
        int i = binsearch(z);
        double dx = z - x[i];
        return b[i] + 2*c[i] * dx;
    }

    double qspline::integ(double z){
        double sum = 0;
        int idx = binsearch(z);
        for (int i = 0; i < idx; i++){
            double dx = x[i+1] - x[i];
            sum += y[i]*dx + 0.5 * b[i]*dx*dx + 1/3 * c[i] * dx*dx*dx;
        }
        double dx = z - x[idx];
        sum += y[idx]*dx + 0.5 * b[idx]*dx*dx + 1/3 * c[idx] * dx*dx*dx;
        return sum;
    }

    cspline::cspline(const vector& x_in, const vector& y_in)
        : n(x_in.size()), x(x_in), y(y_in), b(n), c(n - 1), d(n - 1)    //sidste koefficient i b bruges kun til at beregne c,d
        {
            vector D(n), Q(n-1), B(n), dx(n-1), p(n-1);
            for (int i = 0; i < n - 1; i++) {
                double dy = y[i+1] - y[i];
                dx[i] = x[i+1] - x[i]; p[i] = dy/dx[i];
                // std::cout << p[i] << " " << dx[i];
                if (i == 0){
                    c[i] = 0; D[i] = 2; Q[i] = 1; B[i] = 3 * p[i];
                }
                else{
                    Q[i] = dx[i] / dx[i-1];
                    B[i] = 3 * (p[i-1] + p[i] * Q[i]);
                    D[i] = 2 * Q[i] + 2;
                    D[i] -= Q[i-1] / D[i-1];        //for at få Dtilde
                    B[i] -= B[i-1] / D[i-1];        //for at få Btilde (note at den skal være efter Dtilde)                    
                }

            }
            D[n - 1] = 2;
            B[n - 1] = 3 * p[n-2] - B[n-2]/D[n-2]; 
            b[n - 1] = B[n - 1] / D[n - 1];
                for (int i = n - 2; i >= 0; i--){
                    b[i] = (B[i] - Q[i] * b[i+1]) / D[i];
                    c[i] = (-2*b[i] - b[i+1] + 3 * p[i]) / dx[i];
                    d[i] = (b[i] + b[i+1] - 2*p[i]) / (dx[i] * dx[i]);
                }
        };


    int cspline::binsearch(double z){
        return pp::binsearch(this->x, z);       //kan vist også nøjes med bare at skrive x, syntax for some reason this->x og ikke this.x (noget med pointers i think)
    }                                           //Skal have pp:: da den ellers søger i qspline namespaceog finder sig selv ig
    double cspline::eval(double z){
        int i=binsearch(z);                     //ingen pp her, brug intern funktion
        double dx = z - x[i];
        return y[i] + b[i] * dx + c[i] * dx*dx + d[i] * dx*dx*dx;     //kunne have this-> på alle y,b,x,,c i her i suppose
    }

    double cspline::deriv(double z){
        int i = binsearch(z);
        double dx = z - x[i];
        return b[i] + 2*c[i] * dx + 3*d[i]*dx*dx;
    }

    double cspline::integ(double z){
        double sum = 0;
        int idx = binsearch(z);
        for (int i = 0; i < idx; i++){
            double dx = x[i+1] - x[i];
            sum += y[i]*dx + 0.5 * b[i]*dx*dx + 1/3 * c[i] * dx*dx*dx + 1/4 * d[i] * dx*dx*dx*dx;
        }
        double dx = z - x[idx];
        sum += y[idx]*dx + 0.5 * b[idx]*dx*dx + 1/3 * c[idx] * dx*dx*dx + 1/4 * d[idx] * dx*dx*dx*dx;
        return sum;
    }


} //pp