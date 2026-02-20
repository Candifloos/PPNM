#include<cmath>
#include<numbers> 
#include"sfuns.h"
#include<limits>
#include<vector>


namespace sfuns{
// struct data{int start,end; double sum;};

void harmonic(data& arg){
	int start = arg.start, end = arg.end;
	double sum = 0;
	for (int i = start; i < end; i++) {sum += 1.0/i;}			//1.0 to promote i to double
	arg.sum = sum;
}



}