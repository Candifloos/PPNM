#include"matrix.h"
#include"evd.h"
#include<string>
#include<iostream>
// #include<cmath>
// #include<algorithm>
// #include<cassert>
// #include<stdio.h>
// #include<utility>
// #define SELF (*this)
// #define FORV(i,v) for(int i=0;i<v.size();i++)
// #define FOR_COLS(i,A) for(int i=0;i<A.size2();i++)

namespace pp{
matrix eye(int n){
	matrix M(n,n);
	for(int i=0;i<n;i++)M[i,i]=1;
	return M;
	}

void EVD::timesJ(matrix& A, int p, int q, double theta){
    double c = cos(theta), s = sin(theta);
    for (int i = 0; i < A.size1(); i++){
        double aip = A[i,p], aiq = A[i,q];
        A[i,p] = c * aip - s * aiq;
        A[i,q] = s * aip + c * aiq;
    }
}

void EVD::Jtimes(matrix& A, int p, int q, double theta){
    double c = cos(theta), s = sin(theta);
    for (int i = 0; i < A.size1(); i++){
        double api = A[p,i], aqi = A[q,i];
        A[p,i] =  c * api + s * aqi;
        A[q,i] = -s * api + c * aqi;
    }
}

std::tuple<vector, matrix> EVD::cyclic(matrix M){
    bool changed = true;
    int n = M.size1();
    matrix A = M;
    matrix V = eye(n);
    vector w(n);

    while (changed){
        changed = false;
        for (int p = 0; p < n; p++){
        for (int q = p + 1; q < n; q++){
        double apq = A[p,q], app = A[p,p], aqq = A[q,q];
        double theta = 0.5 * std::atan2(2 * apq, aqq - app);
        double c = std::cos(theta), s = std::sin(theta);
        double new_app = c*c*app - 2*s*c*apq + s*s*aqq;
        double new_aqq = s*s*app + 2*s*c*apq + c*c*aqq;
        if (new_app != app || new_aqq != aqq){
            changed = true;
            timesJ(A,p,q,theta);
            Jtimes(A,p,q,-theta);
            timesJ(V,p,q,theta);
        }
        }}
    }

    for (int i = 0; i < n; i++) w[i] = A[i,i];
    return std::make_tuple(w,V);
}



} //pp





// std::tuple<matrix, matrix> QR::decomp(const matrix& A){
//     int m = A.size2();
//     matrix Q = A;
//     matrix R(m,m);