#include <iostream>
#include <cmath>
#include <iomanip>
#include "min.h"

double Rosenbrock(const pp::vector& v){
    //Rosenbrock f(x,y) = (1-x)^2 + 100(y-x^2)^2
    double x = v[0], y = v[1];
    return (1-x)*(1-x) + 100 * (y-x*x)*(y-x*x);
}
double Himmelblau(const pp::vector& v){
    //f(x,y) = (x^2 + y -11)^2 + (x+y^2-7)^2 
    double x = v[0], y = v[1];
    return std::pow(x*x + y -11, 2) + std::pow(x + y*y - 7, 2);  
}


//Test-structure set up by Gemini and changed somewhat afterwards
void print_row(std::string algo, pp::OptimizationResult res) {
    std::string coords_str = "(" + std::to_string(res.x[0]).substr(0, 5) + ", " 
                                 + std::to_string(res.x[1]).substr(0, 5) + ")";
    std::cout << "| " << std::setw(14) << algo 
              << " | " << std::setw(11)  << res.iterations 
              << " | " << std::setw(8)  << res.f_evals 
              << " | " << std::setw(8)  << res.qr_evals //<< " |\n";
              << " | " << std::setw(15) << coords_str
              << " | " << std::setw(11) << res.f_final << " |\n";
}

void compare_minimizers(const std::string& func_name, const std::function<double(pp::vector)>& func) {
    std::vector<pp::vector> test_points = {
        pp::vector({1.5, 1.5}),
        pp::vector({-2.0, 2.0}),
        pp::vector({-100.0, 100.0}),
        pp::vector({2.0, 100.0}),
    };

    std::cout << "\n====================================================================\n";
    std::cout << " BENCHMARK: " << func_name << "\n";
    std::cout << "====================================================================\n";

    for (const auto& tp : test_points) {
        std::cout << "\nStarting Point: " << " (" << tp[0] << ", " << tp[1] << ")\n";
        std::cout << "| Algorithm      | Iterations  | f_evals  | QR_evals |  Final (x, y)   | Final f(x)  |\n";
        std::cout << "|----------------|-------------|----------|----------|-----------------|-------------|\n";

        auto res_newton = pp::newton(func, tp, 1e-5);
        print_row("Newton + LS", res_newton);

        auto res_newton_with_fallback = pp::newton(func, tp, 1e-3, true);
        print_row("LS + Fallback", res_newton_with_fallback);

        auto res_lm = pp::LMTRDN_minimizer(func, tp, 1e-3);
        print_row("Simple LM", res_lm);

        auto res_lm_adaptive = pp::LMTRDN_adaptive_minimizer(func, tp, 1e-3);
        print_row("Adaptive LM", res_lm_adaptive);
    }
}


void investigate_lambda_influence() {
    std::vector<pp::vector> starting_points = {
        pp::vector({-100.0, 100.0}),
        // pp::vector({-2.0, 2.0}),
        pp::vector({1.5, 1.5}),
        pp::vector({1.1, 0.9})
    };
    std::vector<double> lambda_inputs = {1e-7, 1e-5, 0.001, 0.1, 1.0, 10.0, 100.0, 10000.0, 1e+07};

    // Global header for the gnuplot data file (using # so gnuplot ignores it as a comment)
    std::cerr << "# lambda_0,simple_iters,simple_qr,adapt_iters,adapt_qr\n";

    for (const auto& x0 : starting_points) {
        
        // Isolate the two plotting targets - Not used since plots was deemed redundant (they dont really visualize anything new)
        bool is_point_1 = (std::abs(x0[0] - (-100.0)) < 1e-4 && std::abs(x0[1] - 100.0) < 1e-4);
        bool is_point_2 = (std::abs(x0[0] - 1.1) < 1e-4 && std::abs(x0[1] - 0.9) < 1e-4);
        bool is_target_point = is_point_1 || is_point_2;

        std::cout << "\n=================================================================================\n";
        std::cout << " INFLUENCE OF INITIAL LAMBDA_0 AT (" << x0[0] << ", " << x0[1] << ")\n";
        std::cout << "=================================================================================\n";
        std::cout << "| Lambda_0 | Simple It | Simple f_ev | Simple QR | Adapt It | Adapt f_ev | Adapt QR |\n";
        std::cout << "|----------|-----------|-------------|-----------|----------|------------|----------|\n";

        // Add a block comment marker to stderr to make the raw data file human-readable
        if (is_target_point) {
            std::cerr << "# Dataset: Point (" << x0[0] << ", " << x0[1] << ")\n";
        }

        for (double lambda_0 : lambda_inputs) {
            auto res = LMTRDN_minimizer(Rosenbrock, x0, 1e-3, lambda_0);
            auto res_adap = LMTRDN_adaptive_minimizer(Rosenbrock, x0, 1e-3, lambda_0);
            
            std::cout << "| " << std::setw(8)  << lambda_0 
                      << " | " << std::setw(9)  << res.iterations 
                      << " | " << std::setw(11) << res.f_evals 
                      << " | " << std::setw(9)  << res.qr_evals 
                      << " | " << std::setw(8)  << res_adap.iterations 
                      << " | " << std::setw(10) << res_adap.f_evals 
                      << " | " << std::setw(8)  << res_adap.qr_evals << " |\n";

            if (is_target_point) {
                std::cerr << lambda_0 << " "
                          << res.iterations << " " 
                          << res.qr_evals << " "
                          << res_adap.iterations << " " 
                          << res_adap.qr_evals << "\n";
            }
        }
        std::cout << "=================================================================================\n";

        if (is_target_point) std::cerr << "\n\n";
    }
}


int main(int argc, char** argv) {

    std::cout << "The project implements both a simple Levenberg–Marquardt Trust-Region Damped\n"
              << "Newton (LM) Minimizer and a more advanced version. The two will be referred to\n"
              << "as Simple LM and Adaptive LM. These are compared to a simple Newton\n"
              << "back-tracking linesearch (Newton + LS) algorithm and a similar Newton\n"
              << "but with a fallback to a simple gradient descent if the linesearch fails\n"
              << "(LS + Fallback). All four models are tested first on the Himmelblau\n"
              << "function for different starting points:\n";

    compare_minimizers("Himmelblau", Himmelblau);

    std::cout << "\nWhere I have tracked 3 primary metrics: the number of iterations, function\n"
              << "evaluations (f_evals), and calls to a QR-solver (QR_evals). Each iteration\n"
              << "will evaluate the gradient and Hessian matrix once, causing (1+dim) and\n"
              << "(1+dim)^2 function evaluations respectively; these evaluations are included\n"
              << "in f_evals. We see that the two Newton methods tend to have similar or more\n"
              << "function evaluations, while the LM methods have similar or more QR\n"
              << "evaluations. This is expected behavior as the Newton methods evaluate the\n"
              << "function every time they readjust λ and only call QR once per iteration,\n"
              << "while the LM variants call the QR-solver every time they readjust λ.\n"
              << "Overall, all 4 methods seem to handle the Himmelblau function without many\n"
              << "problems. As a second test, we can try the Rosenbrock function:\n";

    compare_minimizers("Rosenbrock", Rosenbrock);

    std::cout << "\nHere it should be noted that the maximum allowed iterations are 100000.\n"
              << "We here see the Newton methods fail when evaluating far from the minimum at\n"
              << "(-100,100). As this was not seen for the Himmelblau function, it is likely\n"
              << "caused by the x^4 dependence resulting in a x^2 dependence in the Hessian\n"
              << "matrix, making it ill-conditioned at x=100. This is validated by the (2,100) point\n"
              << "where the Newton methods converge again as x is no longer extreme. One can\n"
              << "also note that the fallback to a simple gradient descent for the point\n"
              << "(2,100) lets the Fallback Newton converge way quicker. The gradient descent\n"
              << "fallback was, however, not enough to save the Newton method for the\n"
              << "(-100,100) point, likely due to its rather crude implementation, as we see\n"
              << "that the LM methods, which default to gradient descent with small steps,\n"
              << "safely navigate to the minimum. For the (-2,2) point, we also note again that\n"
              << "while the Newton method uses more function evaluations, it uses fewer\n"
              << "QR-evaluations.\n\n"
              << "In total, when comparing the two, it would seem that the LM methods are\n"
              << "generally more stable with equal/fewer function evaluations and equal/more\n"
              << "QR evaluations. This means that they would be good for complex functions\n"
              << "that are costly to evaluate, but their efficiency would decrease rapidly\n"
              << "with functions in more dimensions as the QR solving process scales as\n"
              << "O(N^3) for square matrices such as the Hessian.\n\n"
              << "We can now investigate the impact of the initial damping parameter λ_0\n"
              << "for both the simple and adaptive LM methods. For this, we will use the\n"
              << "Rosenbrock function as it seems to be the most challenging one in this case.\n"
              << "We'll try different starting points - do note that iterations will be\n"
              << "shortened to It in the table, and Simple / Adapt imply the simple \n"
              << "and adaptive methods respectively.\n";

    investigate_lambda_influence();

    std::cout << "\nGenerally, we see that the method seems to converge no matter the λ_0,\n"
              << "showing the stability of the method. We can also note that when starting at\n"
              << "a reasonable spot, a starting value that is neither too large nor too small\n"
              << "seems to be optimal, as is generally the case. We can also note that the\n"
              << "adaptive method generally uses fewer iterations and QR evaluations than the\n"
              << "simple one. This is also to expectations as the algorithm is a bit more\n"
              << "complex, and is as such generally faster to adapt. For the outlier point at\n"
              << "(-100,100), we see that large λ_0 tend to fare better than the smaller ones.\n"
              << "This is likely due to the previously mentioned ill-conditioned Hessian of\n"
              << "the point, meaning that a large step is likely to fail, and as such,\n"
              << "defaulting to a small gradient descent step as done by the large λ is\n"
              << "generally safer. One could also note that the amount of iterations / QR-\n"
              << "evaluations is nearly constant for low values for the adaptive one. This is\n"
              << "likely due to the extreme scaling of λ when a step fails repeatedly\n"
              << "(λ *= nu; nu *= 2; reset nu when a step succeeds), resulting in λ quickly\n"
              << "becoming large no matter how small it started out. It should be noted that\n"
              << "the simple implementation in some cases actually manages fewer iterations\n"
              << "than the adaptive one; however, generally the amount of QR-evaluations will\n"
              << "still be lower for the adaptive one.\n";

    return 0;
}



