#include"matrix.h"
#include"mc.h"
#include<numbers>
#include <random>

int main(int argc, char** argv){
    double pi = std::numbers::pi;
    int seed = 0;
    bool flag_circ = false; int N_circ = 1000;
    bool flag_standard = false; bool flag_halton = false; int N_halton = 1000;
    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--flag_circ" || arg == "--circ" || arg == "--circ_flag") flag_circ = true;
        if (arg == "--flag_standard" || arg == "--standard" || arg == "--standard_flag") flag_standard = true;
        if (arg == "--flag_halton" || arg == "--halton" || arg == "--halton_flag") flag_halton = true;
        if ((arg == "--N_circ" || arg == "--N_circle") && i+1 < argc) N_circ = (int)std::stod(argv[++i]);
        if (arg == "--N_halton" && i+1 < argc) N_halton = (int)std::stod(argv[++i]);
        if (arg == "--seed" && i+1 < argc) seed = (int)std::stod(argv[++i]);
    }
    pp::lcg rnd(seed);


    std::function<double(pp::vector)> circle  = [](const pp::vector& x){
        if ((x[0]*x[0] + x[1]*x[1]) <= 1) return 1;
        else return 0;
        };
    std::function<double(pp::vector)> ellipsoid = [](const pp::vector& x){
        double a = 1, b = 2, c = 3;
        if ((x[0]*x[0]/(a*a) + x[1]*x[1]/(b*b) + x[2]*x[2]/(c*c)) <= 1) return 1;
        else return 0;
    };
    auto singular_integral = [pi](const pp::vector& x) {
        //from Gemini
        double cos_prod = std::cos(x[0]) * std::cos(x[1]) * std::cos(x[2]);
        double denominator = 1.0 - cos_prod;

        // Singularity check: if denominator is effectively zero, 
        // the value is technically infinite. In MC, we can skip or cap it.
        // if (denominator <= 1e-15) return 0.0; 

        // The factor (1/pi^3) is included here so the result of 
        // mean * V (where V = pi^3) matches the image exactly.
        return (1.0 / (pi*pi*pi)) * (1.0 / denominator);
    };


    if (flag_circ){
        pp::vector a = {-1.0, -1.0};
        pp::vector b = {1.0, 1.0};

        auto [val, err] = pp::plainmc(circle, a, b, N_circ, rnd);

        std::cout << N_circ << " " << val << " " << err << " " << std::abs(val - pi) << "\n";
    }

    if (flag_halton){
    pp::Halton halton(2);
    pp::vector a = {-1.0, -1.0};
    pp::vector b = {1.0, 1.0};
    auto [val, err] = pp::LDmc(circle, a, b, N_halton, halton);
    // std::cout << "Integral of ellipsoid is " << val << " +- " << err << "\n";
    std::cout << N_halton << " " << val << " " << err << " " << std::abs(val - pi) << "\n";
    }


    if (flag_standard){
    pp::vector a_el = {-3.0, -3.0,-3.0};
    pp::vector b_el = {3.0, 3.0, 3.0};
    auto [val, err] = pp::plainmc(ellipsoid, a_el, b_el, 1e5, rnd);
    std::cout << "Integral of ellipsoid is " << val << " ± " << err << "\n";
    std::cout << "Expected " << 1*2*3*4/3*3.1415926535 << "\n \n";
    
    //from gemini
    std::mt19937 engine(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    struct StdWrapper {
        std::mt19937& e;
        std::uniform_real_distribution<double>& d;
        double next() { return d(e); }
    };
    StdWrapper std_rnd{engine, dist};

    pp::vector a = {0, 0, 0};
    pp::vector b = {pi, pi, pi};
    pp::Halton halton(3);
    int samples = (int)1e6;
    auto [val_plain, err_plain] = pp::plainmc(singular_integral, a, b, samples, rnd);
    auto [val_hal, err_hal] = pp::LDmc(singular_integral, a, b, samples, halton);
    auto [val_C, err_C] = pp::plainmc(singular_integral, a, b, samples, std_rnd);
    std::cout << "Integral of (1-cos(x)cos(y)cos(z))^-1 using " << samples << " samples is: \n";
    std::cout << "Plain mc = " << val_plain << " ± " << err_plain << "\n";
    std::cout << "Halton = " << val_hal << " ± " << err_hal << "\n";
    std::cout << "c++ generator = " << val_C << " ± " << err_C << "\n";
    std::cout << "Expected 1.39320 \n";
    std::cout << "Which are generally fairly close. For this particular seed & samples the plain MC seems to hit the closest, although";
    std::cout << "its error estimate is larger than for the Quasi-random one \n";
    
    auto [val_strat, err_strat] = pp::stratified_mc(singular_integral, a, b, samples, 10000, std_rnd);
    // auto [val_strat, err_strat] = pp::stratified_mc(ellipsoid, a_el, b_el, samples, 1000, std_rnd);
    std::cout << "Stratified sampling gives = " << val_strat << " ± " << err_strat << "\n";
    std::cout << "Alligning with previous estimates \n";

    }// flag standard





    return 0;
}
