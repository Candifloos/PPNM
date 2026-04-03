#pragma once
#include"matrix.h"


namespace pp {
    using stepper_type = std::function<std::tuple<vector, vector>(
        const std::function<vector(double, vector)>&, double, const vector&, double)>;

    std::tuple<vector,vector> rkstep12(
	std::function<vector(double,vector)> f, double x, vector y, double h);
    std::tuple<vector,vector> rkstep23(
	std::function<vector(double,vector)> f, double x, vector y, double h);

    std::tuple<std::vector<double>, std::vector<vector>> driver(
    std::function<vector(double, vector)> F,
        double a, 
        double b, 
        vector yinit, 
        stepper_type stepper = rkstep23, // Defaults go in header
        double max_step = 1e20,
        double acc = 0.01, 
        double eps = 0.01,
        double h = 0.125   
    );
}
