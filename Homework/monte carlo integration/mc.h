#pragma once
#include"matrix.h"
#include <random>
#include <functional>
typedef unsigned long long uint64;

namespace pp{    
    class lcg {         //heavily inspired by gemini till "double next()"
    private:            //uint64 to avoid integer overflow
        uint64 seed;
        uint64 a;
        uint64 c;
        uint64 m;

    public:
        lcg(uint64 seed = 0, 
            uint64 a = 1664525, 
            uint64 c = 1013904223, 
            // uint64 m = 4294967296ULL) 
            uint64 m = std::pow(2,32))
            : seed(seed), a(a), c(c), m(m) {}

    double next();
    };

    // std::tuple<double, double> plainmc(std::function<double(vector)> f, const vector& a, const vector& b, int N, lcg& rnd);
    // template<typename Generator>
    // std::tuple<double, double> plainmc(std::function<double(vector)> f, const vector& a, const vector& b, int N, Generator& rnd);
    template<typename Generator>    //template idea from gemini
    std::tuple<double, double> plainmc(std::function<double(vector)> f, const vector& a, const vector& b, int N, Generator& rnd){
        int dim = a.size();
        double V = 1;
        for (int i = 0; i < dim; i++) V *= b[i] - a[i];
        double sum1 = 0, sum2 = 0;
        vector x(dim);
        // vector rnd = lcg(N * dim);

        for (int j = 0; j < N; j++){
            for (int i = 0; i < dim; i++) x[i] = a[i] + rnd.next() * (b[i] - a[i]);
            double fx = f(x); sum1 += fx; sum2 += fx * fx;
        }
        double mean = sum1 / N;
        double sigma = std::sqrt(sum2 / N - mean * mean);
        return std::make_tuple(mean * V, sigma * V / std::sqrt(N));
    }//plainmc
    
    double corput(int n, int b = 2);
    vector prime_numbers(int N);

    class Halton{
        private:
        int dim;
        vector bases;

        public:
        Halton(int d = 1) : dim(d), bases(prime_numbers(d)) {}

        vector x(int n);
    };
    std::tuple<double, double> LDmc(std::function<double(vector)> f, const vector& a, const vector& b, int N, Halton& rnd);





    /*From Gemini*/
    template<typename Generator>
    std::tuple<double, double> stratified_mc(std::function<double(vector)> f, 
                                          vector a, vector b, 
                                          int N, int nmin, Generator& rnd) 
{
    int dim = a.size();
    if (N < nmin) return plainmc(f, a, b, N, rnd);

    // 1. Exploration: Sample nmin points to find sub-variances
    // We calculate variance in the left and right halves of each dimension
    std::vector<double> var_left(dim, 0), var_right(dim, 0);
    std::vector<double> sum_left(dim, 0), sum_right(dim, 0);
    std::vector<int> count_left(dim, 0), count_right(dim, 0);

    for (int i = 0; i < nmin; i++) {
        vector x(dim);
        for (int d = 0; d < dim; d++) x[d] = a[d] + rnd.next() * (b[d] - a[d]);
        double fx = f(x);

        for (int d = 0; d < dim; d++) {
            double mid = (a[d] + b[d]) / 2.0;
            if (x[d] < mid) {
                sum_left[d] += fx; var_left[d] += fx * fx; count_left[d]++;
            } else {
                sum_right[d] += fx; var_right[d] += fx * fx; count_right[d]++;
            }
        }
    }

    // 2. Find the dimension with the largest sub-variance
    int split_dim = 0;
    double max_var = -1;
    for (int d = 0; d < dim; d++) {
        // Calculate variance: <f^2> - <f>^2
        double v_l = (count_left[d] > 0) ? var_left[d]/count_left[d] - pow(sum_left[d]/count_left[d], 2) : 0;
        double v_r = (count_right[d] > 0) ? var_right[d]/count_right[d] - pow(sum_right[d]/count_right[d], 2) : 0;
        
        if (v_l + v_r > max_var) {
            max_var = v_l + v_r;
            split_dim = d;
        }
    }

    // // 3. Subdivide and allocate remaining points
    vector a_left = a, b_left = b;
    vector a_right = a, b_right = b;
    double mid = (a[split_dim] + b[split_dim]) / 2.0;
    b_left[split_dim] = mid;
    a_right[split_dim] = mid;


    // // 4. Recursive Dispatch
    // auto [res_l, err_l] = stratified_mc(f, a_left, b_left, N_left, nmin, rnd);
    // auto [res_r, err_r] = stratified_mc(f, a_right, b_right, N_right, nmin, rnd);
    int N_rem = N - nmin;
    double v_l = std::sqrt(var_left[split_dim]);
    double v_r = std::sqrt(var_right[split_dim]);

    int N_left, N_right;

    if (v_l + v_r > 0) {
        // Proportional allocation
        // We add a tiny floor (e.g., 1/10th of remaining points) so no side gets 0
        double fraction = v_l / (v_l + v_r);
        N_left = static_cast<int>(N_rem * fraction);
    } else {
        // If both variances are 0, split the points 50/50
        N_left = N_rem / 2;
    }
    N_right = N_rem - N_left;

    // 4. Final Safety Check: Never recurse with N=0
    if (N_left < 1) { N_left = 2; } // Need at least some points for a mean/variance
    if (N_right < 1) { N_right = 2; }

    // Dispatch recursive calls
    auto [res_l, err_l] = stratified_mc(f, a_left, b_left, N_left, nmin, rnd);
    auto [res_r, err_r] = stratified_mc(f, a_right, b_right, N_right, nmin, rnd);

    return {res_l + res_r, sqrt(err_l * err_l + err_r * err_r)};
}

}