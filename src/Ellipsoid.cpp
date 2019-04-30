//
// Created by Noè Murr on 08/10/2018.
//

#include "../hdr/navtools/Ellipsoid.hpp"
#include <cmath>
#include <algorithm>

const std::vector<ntl::Ellipsoid>
ntl::Ellipsoid::standards{ // NOLINT(cert-err58-cpp)
    Ellipsoid{6378137.0, 6356752.3142, 1 / 298.257223563, "WGS-84"},
    Ellipsoid{6378137.0, 6378137.0, 0., "sphere"}
};

std::ostream &ntl::operator<<(
    std::ostream &os,
    const ntl::Ellipsoid &ref
) {
    if (!ref.name.empty()) {
        os << "{" << ref.name << " ";
    } else {
        os << "{Unnamed Ellipsoid ";
    }
    os << "a=" << ref.semiMajorAxis << ",b=" << ref.semiMinorAxis
       << ", f=" << ref.flattering << "}";
    return os;
}

bool ntl::Ellipsoid::operator==(
    const ntl::Ellipsoid &rhs
) const {
    return semiMajorAxis == rhs.semiMajorAxis &&
           semiMinorAxis == rhs.semiMinorAxis &&
           flattering == rhs.flattering &&
           eccentricity == rhs.eccentricity &&
           name == rhs.name;
}

bool ntl::Ellipsoid::operator!=(
    const ntl::Ellipsoid &rhs
) const {
    return !(rhs == *this);
}

ntl::Ellipsoid::Ellipsoid(
    const double semiMajorAxis,
    const double semiMinorAxis,
    const double flattering,
    std::string name
) : semiMajorAxis(semiMajorAxis),
    semiMinorAxis(semiMinorAxis),
    flattering(flattering),
    eccentricity(
        sqrt(1 - std::pow(semiMinorAxis, 2.) / std::pow(semiMajorAxis, 2.))
    ),
    name(std::move(name)) {}

const ntl::Ellipsoid &ntl::Ellipsoid::getStd(
    const std::string &name
){
    auto ref = std::find_if(
        standards.begin(),
        standards.end(),
        [name](auto &&p){
            return p.name == name;
        }
    );
    if (ref == standards.end())
        throw std::invalid_argument("unknown standard Ellipsoid!");
    return *ref;
}

double ntl::Ellipsoid::getSemiMajorAxis() const {
    return semiMajorAxis;
}

double ntl::Ellipsoid::getSemiMinorAxis() const {
    return semiMinorAxis;
}

double ntl::Ellipsoid::getFlattering() const {
    return flattering;
}

double ntl::Ellipsoid::getEccentricity() const {
    return eccentricity;
}

const std::string &ntl::Ellipsoid::getName() const {
    return name;
}


