#include"qr.h"
#include"matrix.h"
#include<string>
#include<algorithm>
#include<cmath>
#include<iostream>
#include<cassert>
#include<stdio.h>
#include<utility>
#define SELF (*this)
#define FORV(i,v) for(int i=0;i<v.size();i++)
#define FOR_COLS(i,A) for(int i=0;i<A.size2();i++)

namespace pp{

std::tuple<matrix, matrix> QR::decomp(const matrix& A){
    int m = A.size2();
    matrix Q = A;
    matrix R(m,m);
    /* orthogonalize Q and fill-in R */
    for (int i = 0; i < m; i++){
        R[i,i] = Q[i].norm();            //Q[i] er den i'te søjle
        Q[i] /= R[i,i];
        for (int j = i + 1; j < m; j++){
            R[i,j] = Q[i].dot(Q[j]);
            Q[j] -= Q[i] * R[i,j];
        }
    }
    return std::make_tuple(Q,R);
}

vector QR::solve(matrix& Q, matrix& R, vector b){
    //Rx = Q^T * b, 
    vector newvec = Q.transpose() * b;
    for (int i = newvec.size() - 1; i >= 0; i--){
        double sum = 0;
        for (int j = i + 1; j < newvec.size(); j++){sum += R[i,j] * newvec[j];}
        newvec[i] = (newvec[i] - sum) / R[i,i];
    };
    return newvec;
}

double QR::det(matrix R){
    double product = 1;
    for (int i = 0; i < R.size1(); i++){
        product *= R[i,i];
    }
    return product;
}

matrix QR::inverse(matrix Q,matrix R){
    //Løs Ax_i=e_i for alle i (n dimension), x_i er søjler i A^-1
    //Kan gøres ved at bruge QR::solve mange gange ig?
    int m = R.size1(); int n = R.size2();
    if (!approx(m,n)) {throw std::invalid_argument("Matrix must be square");}
    if (det(R) == 0) {throw std::invalid_argument("Matrix determinant is zero, not invertible");}

    matrix inverseM(m,m); 
    for (int i = 0; i < m; i++){
        vector b(n); b[i] = 1.0;
        // b.print("b = ");
        vector x = solve(Q,R,b);
        // x.print("x = ");
        for (int j = 0; j < m; j++){
            inverseM[j,i] = x[j];               //første er række, næste søjle 
        }
    }
    return inverseM;
}




}//pp

