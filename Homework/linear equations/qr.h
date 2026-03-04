#pragma once
#include<iostream>
#include<cstdio>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<initializer_list>
#include<functional>
#include<ranges>
#include<utility>
#include"matrix.h"


namespace pp{
        class QR {
        public:
        static std::tuple<matrix, matrix> decomp(const matrix& A);
        static vector solve(matrix& Q, matrix& R, vector b);
        static double det(matrix R);
        static matrix inverse(matrix Q,matrix R);
    };
}