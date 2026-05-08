#include"rf.h"

namespace pp{
    void update_Jacobian(std::function<vector(vector)> f, vector& x, const vector& fx, matrix& J){
        int dim = x.size();
        vector dx(dim);
        // matrix J(dim,dim);
        for (int i = 0; i < dim; i++) dx[i] = std::abs(x[i]) * std::pow(2,-26);
        for (int j = 0; j < dim; j++){
            x[j] += dx[j];
            vector df = f(x) - fx;
            for (int i = 0; i < dim; i++){
                J[i,j] = df[i] / dx[j];
                // J[i][j] = df[i] / dx[j];
            }//i loop
            x[j] -= dx[j];
        }//j loop
    }//Jacobian

    // vector newton(std::function<vector(vector)> f, vector x, double acc, double alpha_min, int max_iter){
    //     int dim = x.size();
    //     vector fx = f(x);
    //     matrix J(dim, dim);
    //     for (int _ = 0; _ <= max_iter; _++){
    //         if (fx.norm() < acc) break;
    //         update_Jacobian(f, x, fx, J);
    //         auto [Q,R] = QR::decomp(J);
    //         vector Dx = QR::solve(Q,R, -fx);

    //         double alpha = 1;
    //         vector z = x + alpha*Dx;
    //         vector fz = f(z);
    //         while (true){
    //             z = x + alpha*Dx;
    //             fz = f(z);
    //             if (fz.norm() < fx.norm()) break;
    //             if (alpha < alpha_min) break;
    //             alpha /= 2;
    //         }
    //         x = z;
    //         fx = fz;
    //     }//_ loop
    //     return x;
    // }//newton
    vector newton(std::function<vector(vector)> f, vector x, double acc, double alpha_min, int max_iter){
        int dim = x.size();
        vector fx = f(x);
        matrix J(dim, dim);

        for (int _ = 0; _ <= max_iter; _++){
            double fxnorm = fx.norm();
            if (fxnorm < acc) break;
            update_Jacobian(f, x, fx, J);
            auto [Q,R] = QR::decomp(J);
            vector Dx = QR::solve(Q,R, -fx);
            
            double alpha = 1;
            double g0 = 0.5 * fxnorm * fxnorm, g_prime_0 = -fxnorm*fxnorm;
            vector z, fz;
            while (true){
                z = x + alpha*Dx;
                fz = f(z);
                double fznorm = fz.norm();
                if (fznorm < (1.0 - alpha / 2.0) * fxnorm) break;   //conditions from Gemini - couldnt find quadratic interpolation line-search in notes
                if (alpha < alpha_min) break;

                double g_alpha = 0.5 * fznorm*fznorm;
                double c = (g_alpha - g0 - g_prime_0 * alpha) / (alpha*alpha);

                double alpha_next;      //checks from Gemini
                if (c > 0) {
                    alpha_next = -g_prime_0 / (2.0 * c);
                } else {
                    // Safeguard: if quadratic is concave, default to simple halving
                    alpha_next = alpha / 2.0;
                }
                // Safeguard: don't let alpha shrink too fast or too slow
                if (alpha_next > 0.5 * alpha) {
                    alpha_next = 0.5 * alpha;
                } else if (alpha_next < 0.1 * alpha) {
                    alpha_next = 0.1 * alpha;
                }

                alpha = alpha_next;
            }
            x = z;
            fx = fz;
        }//_ loop
        return x;
    }//newton

}//pp