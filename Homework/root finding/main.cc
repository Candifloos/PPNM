#include"matrix.h"
#include"rf.h"
#include"ode.h"

//Rosenbrock f(x,y) = (1-x)^2 + 100(y-x^2)^2
//=> dfdx = -2(1-x) + 400x^3 - 400yx = -2(1-x) + 400x(x^2 - y)
//=> dfdy = 200(y-x^2)
pp::vector Rosenbrock_grad(const pp::vector& x){
    pp::vector grad(2);
    grad[0] = -2 * (1-x[0]) + 400 * x[0] * (x[0]*x[0] - x[1]);
    grad[1] = 200 * (x[0] - x[1]*x[1]);
    return grad;
}
//Himmelblau f(x,y) = (x^2 + y -11)^2 + (x + y^2 - 7)^2
//=> dfdx = 4x(x^2 + y -11) + 2(x + y^2 - 7)
//=> dydx = 2(x^2 + y - 11) + 4y(x + y^2 - 7)
pp::vector Himmelblau_grad(const pp::vector& x){
    pp::vector grad(2);
    double p1 = (x[0]*x[0] + x[1] - 11), p2 = (x[0] + x[1]*x[1] - 7);
    grad[0] = 4*x[0] * p1 + 2 * p2;
    grad[1] = 2 * p1 + 4 * x[1] * p2;
    return grad;  
}
void part_A(){
    pp::vector x1 = {5,2};
    pp::vector x2 = {500,22};
    pp::vector R_roots1 = pp::newton(Rosenbrock_grad, x1, 1e-3, 1e-3, 1000);
    pp::vector R_roots2 = pp::newton(Rosenbrock_grad, x2, 1e-3, 1e-3, 100000);
    std::cout << "Extremum of Rosenbrock valley function for initial guess (5,2) at x = " << R_roots1[0] << " y = " << R_roots1[1] << "\n";
    std::cout << "Extremum of Rosenbrock valley function for initial guess (500,22) at x = " << R_roots2[0] << " y = " << R_roots2[1] << "\n";
    std::cout << "With gradient values: \n";
    std::cout << Rosenbrock_grad(R_roots1)[0] << " " << Rosenbrock_grad(R_roots1)[1] << "\n";
    std::cout << Rosenbrock_grad(R_roots2)[0] << " " << Rosenbrock_grad(R_roots2)[1] << "\n";

    pp::vector H_roots1 = pp::newton(Himmelblau_grad, x1, 1e-3, 1e-3, 1000);
    pp::vector H_roots2 = pp::newton(Himmelblau_grad, x2, 1e-3, 1e-3, 100000);
    std::cout << "Extremum of Himmelblau function for initial guess (5,2) at x = " << H_roots1[0] << " y = " << H_roots1[1] << "\n";
    std::cout << "Extremum of Himmelblau function for initial guess (500,22) at x = " << H_roots2[0] << " y = " << H_roots2[1] << "\n";
    std::cout << "With gradient values: \n";
    std::cout << Himmelblau_grad(H_roots1)[0] << " " << Himmelblau_grad(H_roots1)[1] << "\n";
    std::cout << Himmelblau_grad(H_roots2)[0] << " " << Himmelblau_grad(H_roots2)[1] << "\n\n";
}

double SE_shooting_method(double r_min, double r_max, pp::vector y_init, pp::vector E_vec, double ODE_acc, double ODE_eps){
    double r_start = r_min, r_end = r_max; pp::vector yinit = y_init; 
    std::function<pp::vector(pp::vector)> M = [r_start, r_end, yinit, ODE_acc, ODE_eps](pp::vector E_vec){ //take energy as vector to work with rf implementation
    double E = E_vec[0];

    auto F = [E](double r, pp::vector y){   //differential equation for ODE
        pp::vector dy(2);
        dy[0] = y[1];
        dy[1] = -2.0 * (E + 1.0/r) * y[0];
        return dy;
    };

    auto [r,f] = pp::driver(F, r_start, r_end, yinit, pp::rkstep23, 1e20, ODE_acc, ODE_eps);        
    pp::vector f_rmax = {f[f.size() - 1][0]};       //take f[-1][0] to get the last value of psi
    return f_rmax;
    };//M

    pp::vector E0 = {-10.0};
    pp::vector E_root = pp::newton(M, E0, 1e-3, 1e-3, 1000);
    // std::cout << "Extremum found for Schrödinger equation M(E) with E = " << E_root[0] << " intial guess being " << E0[0] << "\n";
    return E_root[0];
}


int main(int argc, char** argv){
    bool standard = false, wavefunc = false, r_min_conv = false, r_max_conv = false, ODE_prec_conv = false;
    double r_min = 1e-4, r_max = 8;
    double ODE_acc = 1e-3, ODE_eps = 1e-3;
    
    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--standard") standard = true;
        if (arg == "--wavefunc") wavefunc = true;
        if (arg == "--r_min_conv") r_min_conv = true;
        if (arg == "--r_max_conv") r_max_conv = true;
        if (arg == "--ODE_prec_conv") ODE_prec_conv = true;
        if (arg == "--r_min" && i+1<argc) r_min = std::stod(argv[++i]);
        if (arg == "--r_max" && i+1<argc) r_max = std::stod(argv[++i]);
        if (arg == "--ODE_acc" && i+1<argc) ODE_acc = std::stod(argv[++i]);
        if (arg == "--ODE_eps" && i+1<argc) ODE_eps = std::stod(argv[++i]);

    }
    pp::vector yinit = {r_min - std::pow(r_min, 2), 1.0 - 2.0 * r_min};
    pp::vector E0 = {-0.9}; double E_exact = -0.5;

    if (standard){
        part_A();
        double E = SE_shooting_method(r_min, r_max, yinit, E0, ODE_acc, ODE_eps);
        std::cout << "Extremum found for Schrödinger equation M(E) with E = " << E << " intial guess being " << E0[0] << "\n";
        std::cout << "The convergence plots generally see an increase in E accuracy till they get limited by the other parameters";
    }
        
    if (wavefunc){
        double E = SE_shooting_method(r_min, r_max, yinit, E0, ODE_acc, ODE_eps);

        auto F = [E](double r, pp::vector y){  
        pp::vector dy(2);
        dy[0] = y[1];
        dy[1] = -2.0 * (E + 1.0/r) * y[0];
        return dy;
        };

        auto [r,f] = pp::driver(F, r_min, r_max, yinit, pp::rkstep23, 1e20, ODE_acc, ODE_eps);  
        for (unsigned i = 0; i<r.size(); i++){
            std::cout << r[i] << " " << f[i][0] << "\n";
        }
    }
    if (r_min_conv){
        double E = SE_shooting_method(r_min, r_max, yinit, E0, ODE_acc, ODE_eps);
        std::cout << r_min << " " << std::abs(E - E_exact) << "\n";
    }
    if (r_max_conv){
        double E = SE_shooting_method(r_min, r_max, yinit, E0, ODE_acc, ODE_eps);
        std::cout << r_max << " " << std::abs(E - E_exact) << "\n";
    }
    if (ODE_prec_conv){
        double E = SE_shooting_method(r_min, r_max, yinit, E0, ODE_acc, ODE_eps);
        std::cout << ODE_acc << " " << std::abs(E - E_exact) << "\n";
    }
    


    
    return 0;
}
