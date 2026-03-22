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
    
double vector::dot(const vector& other){
    double product = 0;
    FORV(i, SELF) product += SELF[i] * other[i];
    return product;
}



}//pp

