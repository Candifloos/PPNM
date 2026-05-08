    #include"mc.h"

namespace pp{
    double lcg::next(){
        seed = (a * seed + c) % m;
        return static_cast<double>(seed) / m;
    }

    double corput(int n, int b){
        double q = 0; double bk = 1.0/b;
        while (n>0){q += n % b * bk; n = (int)n/b; bk /= b;};
        return q;
    }//corput


    vector prime_numbers(int N){
        int candidate = 2; vector primes(N); int j = 0;     //j used to avoid allocating new memory all the time
        while (j < N){
            bool candidate_is_prime = true;
            for (int i = 0; i < j; i++){
                if (primes[i] * primes[i] > candidate) break;
                if (std::fmod(candidate, primes[i]) == 0) {candidate_is_prime = false; break;}
            }//i loop
            if (candidate_is_prime) primes[j++] = candidate;
            candidate += 1;
        }//while
        return primes;
    }//prime_numbers

    vector Halton::x(int n){
        int dim = bases.size();
        vector x(dim);
        for (int i = 0; i<dim; i++) x[i] = corput(n,bases[i]);
        return x;
    }


    std::tuple<double, double> LDmc(std::function<double(vector)> f, const vector& a, const vector& b, int N, Halton& rnd){
        int dim = a.size();
        double V = 1;
        for (int i = 0; i < dim; i++) V *= b[i] - a[i];
        double sum1 = 0, sum2 = 0;
        vector x1(dim); vector x2(dim);
        // vector rnd = lcg(N * dim);
        // vector rnd = Halton;

        for (int j = 0; j < N; j++){
            vector rndx1 = rnd.x(j + 1), rndx2 = rnd.x(j + N + 1);
            for (int i = 0; i < dim; i++) {
                x1[i] = a[i] + rndx1[i] * (b[i] - a[i]);
                x2[i] = a[i] + rndx2[i] * (b[i] - a[i]);
            }
            sum1 += f(x1); sum2 += f(x2);
        }
        
        double mean1 = sum1 / N; double mean2 = sum2 / N;
        // double sigma = std::sqrt(sum2 / N - mean * mean);
        double sigma = std::abs(mean1 - mean2);
        return std::make_tuple((mean1+mean2)/2 * V, sigma*V);
    }//plainmc


}//pp

