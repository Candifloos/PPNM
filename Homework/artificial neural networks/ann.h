#pragma once
#include <functional>
#include "matrix.h" // Assumes this defines pp::vector within namespace pp

namespace pp {

    struct ann {
        int n;                            /* number of hidden neurons */
        std::function<double(double)> f;  /* activation function - should always be gaussian wavelet for derivatives to fit */
        vector p;                         /* network parameters */

        // Constructor
        ann(int num_neurons, std::function<double(double)> activation_func);

        // Returns the network response Fp(x)
        double response(double x) const;

        // Trains the network using pp::newton
        void train(const vector& x, const vector& y);


        double derivative(double x) const;
        double derivative2(double x) const;
        double antiderivative(double x) const;
    };

} // namespace pp