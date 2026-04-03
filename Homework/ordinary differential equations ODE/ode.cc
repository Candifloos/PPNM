#include"ode.h"
#include<stdexcept>

namespace pp{
    std::tuple<vector,vector> rkstep12(
	std::function<vector(double,vector)> f /* the f from dy/dx=f(x,y) */
	,double x                    /* the current value of the variable */
	,vector y                    /* the current value y(x) of the sought function */
	,double h                     /* the step to be taken */
	)
{
	vector k0 = f(x,y);              /* embedded lower order formula (Euler) */
	vector k1 = f(x+h/2,y+k0*(h/2)); /* higher order formula (midpoint) */
	vector yh = y+k1*h;              /* y(x+h) estimate */
	vector δy = (k1-k0)*h;           /* error estimate */
	return std::make_tuple(yh,δy);
}


    std::tuple<vector,vector> rkstep23(
	std::function<vector(double,vector)> f /* the f from dy/dx=f(x,y) */
	,double x                    /* the current value of the variable */
	,vector y                    /* the current value y(x) of the sought function */
	,double h                     /* the step to be taken */
	)
{
    double alpha = 0.5, beta = 0.6;
	vector k0 = f(x,y);              /* embedded lower order formula (Euler) */
	vector k1 = f(x+alpha*h, y + alpha * k0 * h); /* higher order formula (midpoint) */
    vector k2 = f(x + beta * h, y + (beta - beta*beta / (2*alpha)) * k0 * h + beta*beta / (2*alpha) * k1 * h);
	// vector yh = y+k1*h;              /* y(x+h) estimate */
    vector yh = y + (1 - 1/(2*alpha) - 1/(3*beta*beta) + 1/(3*alpha*beta))*k0*h + (1/(2*alpha) - 1/(3*alpha*beta))*k1*h + 1/(3*beta*beta)*k2*h;
	vector δy = (k2-k1)*h;           /* error estimate */
	return std::make_tuple(yh,δy);
}


    // std::tuple<std::vector<double>, std::vector<vector>> driver(
    //     std::function<vector(double, vector)> F,
    //     double a, double b,
    //     vector yinit,
    //     double h, double acc, double eps, std::string stepper
    // ) {
    //     double x = a; 
    //     vector y = yinit;
    //     std::vector<double> xlist; xlist.push_back(x);
    //     std::vector<vector> ylist; ylist.push_back(y);

    //     do {
    //         if (x >= b) return std::make_tuple(xlist, ylist);
    //         if (x + h > b) h = b - x;
        
    //         vector yh, δy; 
    //         if (stepper == "RK12") {
    //             auto [tmp_yh, tmp_dy] = rkstep12(F, x, y, h);
    //             yh = tmp_yh; δy = tmp_dy;
    //         } 
    //         else if (stepper == "RK23") {
    //             auto [tmp_yh, tmp_dy] = rkstep23(F, x, y, h);
    //             yh = tmp_yh; δy = tmp_dy;
    //         } 
    //         else {
    //             throw std::runtime_error("stepper should be either RK12 or RK23");
    //         }
    //         // auto [yh, δy] = rkstep23(F, x, y, h);

    //         // Tolerance scaling
    //         double tol = (acc + eps * yh.norm()) * std::sqrt(h / (b - a));
    //         double err = δy.norm();

    //         if (err <= tol) { // Step accepted
    //             x += h; 
    //             y = yh;
    //             xlist.push_back(x);
    //             ylist.push_back(y);
    //         }

    //         // Step size adjustment
    //         if (err > 0) {
    //             h *= std::min(std::pow(tol / err, 0.25) * 0.95, 2.0);
    //         } else {
    //             h *= 2.0;
    //         }
            
    //         // Safety check to prevent infinite loops if h becomes tiny
    //         // if (std::abs(h) < 1e-15) {
    //         //     // You might want to throw an error or break here
    //         //     return std::make_tuple(xlist, ylist);
    //         // }
    //     } while (true);
    // }//driver
       std::tuple<std::vector<double>, std::vector<vector>> driver(
        std::function<vector(double, vector)> F,
        double a, double b,
        vector yinit, stepper_type stepper, double max_step,
        double h, double acc, double eps
    ) {
        double x = a; 
        vector y = yinit;
        std::vector<double> xlist; xlist.push_back(x);
        std::vector<vector> ylist; ylist.push_back(y);

        do {
            if (x >= b) return std::make_tuple(xlist, ylist);
            if (x + h > b) h = b - x;
        
            // vector yh, δy; 
            // auto [yh, δy] = rkstep23(F, x, y, h);
            auto [yh, δy] = stepper(F, x, y, h);

            // Tolerance scaling
            double tol = (acc + eps * yh.norm()) * std::sqrt(h / (b - a));
            double err = δy.norm();

            if (err <= tol) { // Step accepted
                x += h; 
                y = std::move(yh);      //move instead of copying
                xlist.push_back(x);
                ylist.push_back(y);
            }

            // Step size adjustment
            if (err > 0) {
                h *= std::min(std::pow(tol / err, 0.25) * 0.95, 2.0);
            } else {
                h *= 2.0;
            }
            h = std::min(h, max_step);
            
            // Safety check to prevent infinite loops if h becomes tiny
            if (std::abs(h) < 1e-15) {
                // You might want to throw an error or break here
                throw std::runtime_error("step size become too small");
                return std::make_tuple(xlist, ylist);
            }
        } while (true);
    }//driver



}