#include"matrix.h"
#include"spline.h"
#include<fstream>

void writeData(std::string output, const pp::vector& x, const pp::vector& y, const pp::vector& z, const pp::vector& y_interp, 
    const pp::vector& y_integ, const pp::vector& y_diff = pp::vector(0)){
    /*Function for writing interpolated data to an output file*/
    std::ofstream outData(output);
    int n = x.size(); int m = z.size();
    for (int i = 0; i < n; i++){
        outData << x[i] << " " << y[i] << "\n";
    }
    outData << "\n\n";
    if (y_diff.size() == 0){
        for (int i = 0; i < m; i++){
            outData << z[i] << " " << y_interp[i] << " " << y_integ[i] << "\n";
        }
    }
    else{
        for (int i = 0; i < m; i++){
            outData << z[i] << " " << y_interp[i] << " " << y_integ[i] << " " << y_diff[i] << "\n";
        } 
    };
    outData.close();
}

void quadDebug(int interpfac){
    int m = 5;
    std::cout << "Quad spline debugging, values are for y_i = 0, y_i = x_i & y_i = x_i^2 respectively \n";
    for (int j = 0; j <= 2; j++){
        pp::vector x_deb(m), y_deb(m), z_deb(m * (interpfac - 0.5) + 1), y_quad_deb(m * (interpfac - 0.5) + 1);
        std::cout << "x y \n";
        for (int i = 0; i < m; i++){
            x_deb[i] = i+1;
            if (j == 2) y_deb[i] = x_deb[i] * x_deb[i];
            else y_deb[i] = x_deb[i]*j;
            // std::cout << x_deb[i] << " " << y_deb[i] << "\n";
        }
        // std::cout << "\n";
        pp::qspline qspline_deb(x_deb,y_deb);
        std::cout << "b c \n";
        for (int i = 0; i < qspline_deb.b.size(); i++){
            std::cout << qspline_deb.b[i] << " " << qspline_deb.c[i] << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "Expected values are given by \n c_i = (dy/dx)_(i+1) - (dy/dx)_i - c_(i-1) with c_0 = 0 \n b_i = dy/dx - c_i \n";
    std::cout << "In all cases dx = 1. In the first case dy = 0 so all b,c should just be zero. \n";
    std::cout << "For y_i=x_i we have dy = 1 for all i, so all c should still be zero while all b should be 1. \n";
    std::cout << "For y_i = x_i^2 we have \n c0 = 0, c1 = 5 - 3 - 0 = 2, c2 = 7 - 5 - 2 = 0, c3 = 9 - 7 - 0 = 2 \n";
    std::cout << "b0 = 3 - 0 = 3, b1 = 5 - 2 = 3, b2 = 7 - 0 = 7, b3 = 9 - 2 = 7 \n";
    std::cout << "Aligning with found values. \n";
}


int main(){
    int n = 19; int interpfac = 10;
    pp::vector x(n), y(n), z(n * (interpfac - 0.5) + 1), y_lin(n * (interpfac - 0.5) + 1), y_lin_int(n * (interpfac - 0.5) + 1);
    for (int i = 0; i < n; i++){
        x[i] = i * 0.5;
        y[i] = std::cos(x[i]);
    }
    for (int i = 0; i < n * (interpfac - 0.5); i++) {
        z[i] = i * (0.5) / interpfac;
        y_lin[i] = pp::linterp(x,y,z[i]);
        y_lin_int[i] = pp::linterpInteg(x,y,z[i]);
    }
    
    writeData("lindata.txt", x, y, z, y_lin, y_lin_int);

    pp::vector y_quad(n * (interpfac - 0.5) + 1), y_quad_int(n * (interpfac - 0.5) + 1), y_quad_diff(n * (interpfac - 0.5) + 1);
    pp::qspline qspline(x,y);
    
    for (int i = 0; i < n * (interpfac - 0.5); i++) {
        y_quad[i] = qspline.eval(z[i]);
        y_quad_int[i] = qspline.integ(z[i]);
        y_quad_diff[i] = qspline.deriv(z[i]);
    }
    writeData("quaddata.txt", x, y, z, y_quad, y_quad_int, y_quad_diff);
    quadDebug(interpfac);



    pp::vector y_cub(n * (interpfac - 0.5) + 1), y_cub_int(n * (interpfac - 0.5) + 1), y_cub_diff(n * (interpfac - 0.5) + 1);
    pp::cspline cspline(x,y);
    
    for (int i = 0; i < n * (interpfac - 0.5); i++) {
        y_cub[i] = cspline.eval(z[i]);
        y_cub_int[i] = cspline.integ(z[i]);
        y_cub_diff[i] = cspline.deriv(z[i]);
    }
    writeData("cubedata.txt", x, y, z, y_cub, y_cub_int, y_cub_diff);



return 0;
}