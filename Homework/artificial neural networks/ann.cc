#include "ann.h"
#include "min.h"  
#include <cmath>

namespace pp {

    ann::ann(int num_neurons, std::function<double(double)> activation_func)    //from Gemini
        : n(num_neurons), f(activation_func), p(3 * num_neurons) {}

    double ann::response(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            
            // // Safeguard against division by zero
            // if (std::abs(b) < 1e-9) b = (b >= 0) ? 1e-9 : -1e-9;

            sum += f((x - a) / b) * w;
        }
        return sum;
    }

    void ann::train(const vector& x, const vector& y) {
        int data_size = x.size();

        // Heuristic initialization of parameters based on the input boundaries - from Gemini
        double xmin = x[0], xmax = x[0];
        for (int i = 0; i < data_size; ++i) {
            if (x[i] < xmin) xmin = x[i];
            if (x[i] > xmax) xmax = x[i];
        }
        double range = xmax - xmin;
        if (range == 0) range = 1.0;

        for (int i = 0; i < n; ++i) {
            p[3 * i]     = xmin + range * i / (n > 1 ? n - 1 : 1); // Distribute centers
            p[3 * i + 1] = range / n;                             // Scale widths
            p[3 * i + 2] = 1.0;                                   // Weights
        }

        auto cost_function = [this, &x, &y, data_size](vector trial_p) -> double {
            double total_cost = 0.0;
            
            for (int k = 0; k < data_size; ++k) {
                double network_output = 0.0;
                for (int i = 0; i < n; ++i) {
                    double a = trial_p[3 * i];
                    double b = trial_p[3 * i + 1];
                    double w = trial_p[3 * i + 2];
                    if (std::abs(b) < 1e-9) b = (b >= 0) ? 1e-9 : -1e-9;    //avoid /0

                    network_output += f((x[k] - a) / b) * w;
                }
                double residual = network_output - y[k];
                total_cost += residual * residual;
            }
            return total_cost;
        };

        OptimizationResult result = newton(cost_function, p, 1e-3);
        
        // Update the internal parameters with optimized values
        p = result.x;
    }



    double ann::derivative(double x) const {    //note that derivatives are only accurate for Gaussian wavelet
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            if (std::abs(b) < 1e-9) b = (b >= 0) ? 1e-9 : -1e-9;

            double u = (x - a) / b;
            double df_du = (1.0 - 2.0 * u * u) * std::exp(-u * u);
            sum += (df_du / b) * w;
        }
        return sum;
    }

    double ann::derivative2(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            if (std::abs(b) < 1e-9) b = (b >= 0) ? 1e-9 : -1e-9;

            double u = (x - a) / b;
            double d2f_du2 = (4.0 * u * u * u - 6.0 * u) * std::exp(-u * u);
            sum += (d2f_du2 / (b * b)) * w;
        }
        return sum;
    }

    double ann::antiderivative(double x) const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            double a = p[3 * i];
            double b = p[3 * i + 1];
            double w = p[3 * i + 2];
            if (std::abs(b) < 1e-9) b = (b >= 0) ? 1e-9 : -1e-9;

            double u = (x - a) / b;
            double integral_f = -0.5 * b * std::exp(-u * u);
            sum += integral_f * w;
        }
        return sum;
    }





} // namespace pp