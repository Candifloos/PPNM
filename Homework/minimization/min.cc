#include"min.h"

namespace pp{

    vector gradient(std::function<double(vector)> f, vector x){
        int dim = x.size();
        double fx = f(x);
        vector gf(dim);
        for (int i = 0; i < dim; i++){
            double dxi = (1+std::abs(x[i])) * std::pow(2,-26);
            x[i] += dxi;
            gf[i] = (f(x) - fx) / dxi;
            x[i] -= dxi;
        }
        return gf;
    }    

    matrix hessian(std::function<double(vector)> f, vector x){
        int dim = x.size();
        matrix H(dim,dim);
        vector gfx = gradient(f,x);
        for (int j = 0; j < dim; j++){
            double dxj = (1+std::abs(x[j])) * std::pow(2,-13);
            x[j] += dxj;
            vector dgf = gradient(f,x) - gfx;
            for (int i = 0; i < dim; i++) H[i,j] = dgf[i] / dxj;
            x[j] -= dxj;
        }
        return H;
    }

    // struct OptimizationResult{  //idea to hold results in seperate struct from Gemini
    //     vector x;
    //     int steps = 0;
    // };

    // vector newton(std::function<double(vector)> f, vector x, double acc){
    //     while (true){
    //         vector g = gradient(f, x);
    //         if (g.norm() < acc) break;  //done
    //         matrix H = hessian(f,x);
    //         for (int i = 0; i < H.size1(); i++) H[i,i] += 1e-6;     //Levenberg regularization
    //         auto [Q,R] = QR::decomp(H);
    //         vector dx = QR::solve(Q,R, -g);
    //         double lam = 1.0;
    //         while (lam >= 1/1024){
    //             if (f(x+lam*dx) < f(x)) break;
    //             lam /= 2;
    //         }
    //         x = x+ lam*dx;
    //     }//while true
    //     return x;
    // }//newton
    OptimizationResult newton(std::function<double(vector)> f, vector x, double acc){  //idea to hold results in seperate struct from Gemini
        int steps = 0;
        while (true){
            vector g = gradient(f, x);
            if (g.norm() < acc) break;  //done
            matrix H = hessian(f,x);
            for (int i = 0; i < H.size1(); i++) H[i,i] += 1e-6;     //Levenberg regularization
            auto [Q,R] = QR::decomp(H);
            vector dx = QR::solve(Q,R, -g);
            // double lam = 1.0;
            // while (lam >= 1/1024){
            //     if (f(x+lam*dx) < f(x)) break;
            //     lam /= 2;
            // }
            // x = x+ lam*dx;
            // steps++;
            double lam = 1.0;       //Gemini adjustment to above for better stability
            bool success = false;
            while (lam >= 1.0 / 1024.0) {
                if (f(x + lam * dx) < f(x)) {
                    success = true;
                    break;
                }
                lam /= 2.0;
            }

            if (success) {
                x = x + lam * dx;
            } else {
                // Fallback: The Newton direction was broken. Take a tiny gradient step instead.
                x = x - 0.01 * g; 
            }
            steps++;

        }//while true
        return {x, steps};
    }//newton
}