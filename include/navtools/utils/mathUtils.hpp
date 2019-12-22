//
// Created by Noè Murr on 28/08/2017.
//

#ifndef NAVTOOLS_MATH_UTILS_HPP
#define NAVTOOLS_MATH_UTILS_HPP

#include <cmath>
#include <stdexcept>

namespace ntl::utils {
    constexpr double toDegree(double radians) {
        return radians * 180.0 / M_PI;
    }

    constexpr double toRadians(double degree) {
        return degree / 180 * M_PI;
    }

    constexpr double round(double value, unsigned int places){
        if (places == 0) return ::round(value);
        return ::round(value * std::pow(10, places)) / std::pow(10, places);
    }

    constexpr double mToNm(double m) {
        return m / 1852;
    }

    constexpr double kmToNm(double km) {
        return mToNm(km * 1000);
    }
}


#endif //NAVTOOLS_MATH_UTILS_HPP
