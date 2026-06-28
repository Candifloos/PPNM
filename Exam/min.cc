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
    //1 + dim f evaluations

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
    //1 + dim gradient evaluations = (1+dim)(1+dim) function evalutions

    OptimizationResult newton(std::function<double(vector)> f, vector x, double acc, bool gradient_fallback){  //idea to hold results in seperate struct from Gemini
        int steps = 0, f_evals = 0, qr_evals = 0, dim = x.size(); 
        double fx = f(x); f_evals++;
        double fx_trial;
        int grad_function_calls = 1+dim, Hess_function_calls = (1+dim)*grad_function_calls;

        while (steps < 100000){
            vector g = gradient(f, x);
            if (g.norm() < acc) break;  //done
            matrix H = hessian(f,x);
            for (int i = 0; i < H.size1(); i++) H[i,i] += 1e-6;     //Levenberg regularization
            auto [Q,R] = QR::decomp(H);     qr_evals++;
            vector dx = QR::solve(Q,R, -g);

            double lam = 1.0;      
            bool success = false;
            while (lam >= 1.0 / 1024.0) {
                fx_trial = f(x + lam * dx); f_evals++;
                if (fx_trial < fx) {
                    success = true;
                    break;
                }
                lam /= 2.0;
            }
            fx = fx_trial;

            if (gradient_fallback){
            if (success) x = x + lam * dx; 
            else {
                    //Fallback: The Newton direction was broken. Take a tiny gradient step instead.
                    x = x - 0.01 * (g/g.norm()); 
                    fx = f(x); f_evals++;
                }
            }
            else{x = x + lam * dx;};

            steps++;
            f_evals += grad_function_calls + Hess_function_calls; //calls both once per loop and the grad one extra time accounted for outside

        }//while true
        f_evals += grad_function_calls; //for the one extra gradient function call compared to steps

        return {x, f(x), steps, f_evals, qr_evals};
    }//newton


    OptimizationResult LMTRDN_minimizer(std::function<double(vector)> f, vector x, double acc, double lambda0) {
        int steps = 0, f_evals = 0, qr_evals = 0, dim = x.size(); 
        // double lambda = 0.1; 
        double fx = f(x); f_evals++;
        double fx_trial;
        int grad_function_calls = 1+dim, Hess_function_calls = (1+dim)*grad_function_calls;
        double lambda = lambda0;

        while (steps < 100000) {
            vector g = gradient(f, x);
            if (g.norm() < acc) break;  //done
            
            matrix H = hessian(f, x);
            
            bool success = false;
            while (!success) {
                matrix H_damped = H; 

                for (int i = 0; i < H_damped.size1(); i++) H_damped[i, i] += lambda;                 
                auto [Q,R] = QR::decomp(H_damped); qr_evals++;
                vector dx = QR::solve(Q,R, -g);
                
                vector x_trial = x + dx;
                
                fx_trial = f(x_trial); f_evals++;
                if (fx_trial < fx) {    //Note states \lambda <- \lambda/2 but should probably be -> considering the text & a quick wiki search
                    x = x_trial;      
                    lambda /= 2.0;  
                    success = true; 
                    fx = fx_trial;
                } else {
                    lambda *= 2.0; 
                }

                // Safeguard to prevent an infinite loop if lambda blows up
                if (lambda > 1e10) {
                    break; 
                }
            }
            steps++;
            f_evals += grad_function_calls + Hess_function_calls; //calls both once per loop and the grad one extra time accounted for outside

        }//while true
        f_evals += grad_function_calls; //for the one extra gradient function call compared to steps

        return {x, f(x), steps, f_evals, qr_evals};
    }//LMTRDN

    OptimizationResult LMTRDN_adaptive_minimizer(std::function<double(vector)> f, vector x, double acc, double lambda0) {
        int steps = 0, f_evals = 0, qr_evals = 0, dim = x.size(); 
        // double lambda = 0.1; 
        double fx = f(x); f_evals++;
        double fx_trial;
        int grad_function_calls = 1+dim, Hess_function_calls = (1+dim)*grad_function_calls;
        double lambda = lambda0;
        double nu = 2.0;

        while (steps < 100000) {
            vector g = gradient(f, x);
            if (g.norm() < acc) break;  //done
            
            matrix H = hessian(f, x);
            
            bool success = false;
            while (!success) {
                matrix H_damped = H; 

                for (int i = 0; i < H_damped.size1(); i++) H_damped[i, i] += lambda;                 
                auto [Q,R] = QR::decomp(H_damped); qr_evals++;
                vector dx = QR::solve(Q,R, -g);
                
                vector x_trial = x + dx;
                
                fx_trial = f(x_trial); f_evals++;

                double actual_reduction = fx_trial - fx;
                //we use Taylor expansion so predicted change should be grad*dx + 1/2*Hess*dx^2 (which should probably be written with indices or proper matrix notation)
                double g_dot_dx = 0;
                for (int i = 0; i < dim; ++i) g_dot_dx += g[i] * dx[i];

                double dx_H_dx = 0;             //loop from Gemini
                for (int i = 0; i < dim; ++i) {
                    double H_dx_i = 0;
                    for (int j = 0; j < dim; ++j) {
                        H_dx_i += H[i, j] * dx[j]; 
                    }
                    dx_H_dx += dx[i] * H_dx_i;
                }
                double pred_reduction = g_dot_dx + 0.5 * dx_H_dx;
                double ratio = (pred_reduction < 0) ? (actual_reduction / pred_reduction) : -1.0;    //turns out there is a smart way to do if/else shorthand. From Gemini from previous homeworks


                if (ratio > 0.0) {  //method from https://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf equation 2.21 METHODS FOR NON-LINEAR LEAST SQUARES PROBLEMS 2nd Edition, April 2004 K. Madsen, H.B. Nielsen, O. Tingleff
                    x = x_trial;      
                    fx = fx_trial;
                    success = true; 

                    double factor = std::max(1.0 / 3.0, 1.0 - std::pow((2.0 * ratio - 1.0), 3));
                    lambda *= factor;
                    nu = 2.0;
                }
                else{
                    lambda *= nu;
                    nu *= 2;
                }
                // if (ratio > 0.75) {           //ALTERNATE TO ABOVE - sugestion to split into great agreement (rho>0.75),bad agreement (rho<0.25) and good agreement in between, from same article
                //     lambda /= 3;
                // } else if (ratio < 0.25) {
                //     lambda *= 4;
                // }
                // if (ratio > 0.0) { 
                //     x = x_trial;      
                //     fx = fx_trial;
                //     success = true; 
                // }

                //Safeguard to prevent an infinite loop if lambda blows up
                if (lambda > 1e10) {
                    break; 
                }
            }
            steps++;
            f_evals += grad_function_calls + Hess_function_calls; //calls both once per loop and the grad one extra time accounted for outside

        }//while true
        f_evals += grad_function_calls; //for the one extra gradient function call compared to steps

        return {x, f(x), steps, f_evals, qr_evals};
    }//LMTRDN adaptive



}//pp