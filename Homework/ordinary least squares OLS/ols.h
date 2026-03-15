#pragma once
#include"qr.h"

namespace pp{
    // vector lsfit(const std::vector<std::function<double(double)>>& fs, const pp::vector& x, const pp::vector& y, const pp::vector& dy);
    std::tuple<vector,matrix> lsfit(const std::vector<std::function<double(double)>>& fs, const pp::vector& x, const pp::vector& y, const pp::vector& dy);
    
}