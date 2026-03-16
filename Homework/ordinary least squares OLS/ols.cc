#include "qr.h"
#include "ols.h"

namespace pp {
    matrix eye(int n){
        matrix M(n,n);
        for(int i=0;i<n;i++)M[i,i]=1;
	    return M;
	}

    std::tuple<vector, matrix> lsfit(const std::vector<std::function<double(double)>>& fs, const vector& x, const vector& y, const vector& dy) {
        int m = fs.size(); int n = y.size();
        
        vector b(n);
        matrix A(n, m);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                A[i,j] = fs[j](x[i]) / dy[i];
            }//j
            b[i] = y[i] / dy[i];
        }//i
        
        auto [Q,R] = QR::decomp(A);
        vector cs = QR::solve(Q,R,b);

        matrix I = eye(m);
        matrix Rinverse = QR::inverse(I,R);
        matrix covariance_matrix = Rinverse * Rinverse.transpose();


        return std::make_tuple(cs, covariance_matrix);
    }//lsfit
}//pp