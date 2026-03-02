#pragma once
#include<iostream>
#include<cmath>
#include"matrix.h"
#include<utility>

// public 
namespace pp{
struct EVD{      //stadig gør at methods & stuff hører til klassen & ikke enkelte objekter af klassen?
    static void timesJ(matrix& A, int p, int q, double theta);//{/*...*/}
    static void Jtimes(matrix& A, int p, int q, double theta);//{/*...*/}
    static std::tuple<vector, matrix> cyclic(matrix M);

};


} //pp namespace