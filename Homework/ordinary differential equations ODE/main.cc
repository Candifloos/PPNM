#include"matrix.h"
#include"ode.h"
#include<fstream>


void writeData(const std::vector<double>& x, const std::vector<pp::vector>& y){
    /*Function for writing data to an output file*/
    int n = x.size();
    for (int i = 0; i < n; i++){
        std::cout << x[i] << " " << y[i][0] << " " << y[i][1] << "\n";
    }
}

int main(int argc, char** argv){
    pp::stepper_type stepper = pp::rkstep23;        //default stepper is RK23
    double eps = 0, x0 = 0, xmax = 50, max_step = 1e8;
    bool oscillator_flag = false, precession_flag = false, tbo_flag = false; 
    pp::vector u0(2); u0[0] = 1; u0[1] = 0;
    for (int i = 0; i < argc; i++){
        std::string arg = argv[i];
        if (arg == "--stepper" && i+1 < argc && argv[++i] == "RK12") stepper = pp::rkstep12;    //can set stepper to RK12 if desired
        if ((arg == "--epsilon" || arg == "--eps") && i+1 < argc) eps = std::stod(argv[++i]);
        if ((arg == "--precession_flag" || arg == "--precession")) precession_flag = true; 
        if ((arg == "--oscillator_flag" || arg == "--oscillator")) oscillator_flag = true; 
        if ((arg == "--tbo_flag" || arg == "--tbo")) tbo_flag = true; 
        if (arg == "--x0" && i+1 < argc) x0 = std::stod(argv[++i]);
        if (arg == "--xmax" && i+1 < argc) xmax = std::stod(argv[++i]);
        if ((arg == "--max_step") && i+1 < argc) max_step = std::stod(argv[++i]);
        if ((arg == "--u0" || arg == "--init") && i+2 < argc) {u0[0] = std::stod(argv[++i]); u0[1] = std::stod(argv[++i]);}
    }

    auto oscillator = [](double x, const pp::vector& y){       //oscillator with friction
        pp::vector res(y.size());
        double b = 0.25, c = 5.0;
        res[0] = y[1];
        res[1] = -b * y[1] - c * std::sin(y[0]);
        return res;
    };
    auto precession = [eps](double x, const pp::vector& y){
        /*y = (u,u') = (u', 1-u + eps*u*u)*/
        pp::vector res(y.size());
        res[0] = y[1];
        res[1] = 1 - y[0] + eps * y[0]*y[0];
        return res;
    };

    auto tbo = [](double x, const pp::vector & z){
        /*Three-body orbit with m_1=m_2=m_3=G=1 => y = (v,v,v,x,x,x) = (a,a,a, v,v,v)*/
        pp::vector res(z.size());
        double x1 = z[6], y1 = z[7], x2 = z[8], y2 = z[9], x3 = z[10], y3 = z[11];

        auto force = [](double xi, double yi, double xj, double yj){
            double dx = xj - xi, dy = yj - yi;
            double dr = std::pow(dx*dx + dy*dy, 0.5), dr3 = dr*dr*dr;
            return std::make_tuple(dx/dr3, dy/dr3);        
        };

        auto [f12x, f12y] = force(x1, y1, x2, y2);
        auto [f13x, f13y] = force(x1, y1, x3, y3);
        auto [f23x, f23y] = force(x2, y2, x3, y3);

        res[0] = f12x + f13x; res[1] = f12y + f13y;     //first body x component sum of x-component forces on it and so forth
        res[2] = -f12x + f23x; res[3] = -f12y + f23y;   //f flips sign since it was computed 1-2 but needs 2-1
        res[4] = -f13x - f23x; res[5] = -f13y - f23y;

        for (int i = 6; i < z.size(); i++) res[i] = z[i - 6];

        return res;
    };

    if (oscillator_flag){
    pp::vector y0(2); y0[0] = 3; y0[1] = 0;
    auto [x,y] = pp::driver(oscillator, x0, xmax, y0, stepper, max_step);
    writeData(x, y);
    };

    if (precession_flag){
    auto [phi,u] = pp::driver(precession, x0, xmax, u0, stepper, max_step); 
    writeData(phi,u);
}

    if (tbo_flag){
    pp::vector z0(12); 
    z0[0] = 0.466203685; z0[1] = 0.43236572; z0[2] = 0.466203685; z0[3] = 0.43236573; z0[4] = -0.93240737; z0[5] = -0.86473146;
    z0[6] = 0.97000436; z0[7] = -0.24308753; z0[8] -= z0[6]; z0[9] -= z0[7]; //last two are zero
    auto [t,z] = pp::driver(tbo, x0, xmax, z0, stepper, max_step); 
    int n = t.size();
    for (int i = 0; i < n; i++){
        std::cout << t[i] << " " << z[i][6] << " " << z[i][7] << " " << z[i][8] << " " << z[i][9] << " " << z[i][10] << " " << z[i][11] << "\n";
    }
    }

return 0;
}