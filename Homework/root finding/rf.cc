#include"rf.h"

namespace pp{

    matrix Jacobian(std::function<vector(vector)> f, vector x, vector fx){
        int dim = x.size();
        vector dx(dim);
        matrix J(dim,dim);
        for (int i = 0; i < dim; i++) dx[i] = std::abs(x[i]) * std::pow(2,-26);
        for (int j = 0; j < dim; j++){
            // dx[j] = std::abs(x[j]) * std::pow(2,-26);
            x[j] += dx[j];
            vector df = f(x) - fx;
            for (int i = 0; i < dim; i++){
                J[i,j] = df[i] / dx[j];
                // J[i][j] = df[i] / dx[j];
                x[j] -= dx[j];
            }//i loop
        }//j loop
        return J;
    }//Jacobian

    vector newton(std::function<vector(vector)> f, vector x, double acc, double alpha_min, int max_iter){
        vector fx = f(x);
        for (int _ = 0; _ <= max_iter; _++){
            if (fx.norm() < acc) break;
            matrix J = Jacobian(f,x,fx);
            auto [Q,R] = QR::decomp(J);
            vector Dx = QR::solve(Q,R, -fx);
            double alpha = 1;
            vector z = x + alpha*Dx;
            vector fz = f(z);
            while (true){
                z = x + alpha*Dx;
                fz = f(z);
                if (fz.norm() < fx.norm()) break;
                if (alpha < alpha_min) break;
                alpha /= 2;
            }
            x = z;
            fx = fz;
        }//_ loop
        return x;
    }//newton

}//pp