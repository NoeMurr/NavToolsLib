//
// Created by Noè Murr on 28/08/2017.
//

#include "../hdr/navtools/Latitude.hpp"
#include "../hdr/navtools/utils/mathUtils.hpp"
#include <cmath>
#include <fmt/format.h>

ntl::Latitude::Latitude(
    int deg, int min, double sec, char sign
) noexcept(false) : DegreeValue(90, deg, min, sec, sign == 'N') {}

bool ntl::Latitude::operator==(const ntl::Latitude &o) const {
    return value == o.value;
}

bool ntl::Latitude::operator!=(const ntl::Latitude &o) const {
    return !(*this == o);
}

bool ntl::Latitude::operator>(const ntl::Latitude &o) const {
    return value > o.value;
}

bool ntl::Latitude::operator<(const ntl::Latitude &o) const {
    return value < o.value;
}

bool ntl::Latitude::operator<=(const ntl::Latitude &o) const {
    return value <= o.value;
}

bool ntl::Latitude::operator>=(const ntl::Latitude &o) const {
    return value >= o.value;
}

char ntl::Latitude::getSign() const noexcept {
    return isSign() ? 'N' : 'S';
}

void ntl::Latitude::setSign(char sign) noexcept {
    DegreeValue::setSign(sign == 'N');
}

double ntl::Latitude::getMeridionalPart(const Ellipsoid &ref) const noexcept {
    return utils::toDegree(getMeridionalPartInRadians(ref))*60;
}

ntl::Latitude::Latitude(double value) : DegreeValue(90, value) {}

ntl::Latitude
ntl::Latitude::approxFromMeridionalPart(
    double mp,
    const Ellipsoid &ref
) {
    using namespace ntl::utils;
    const auto a = 21600. / (2*M_PI);
    const auto tol = 1e-6;
    const auto sign = mp > 0;
    mp = std::abs(mp);
    auto fa =
        2 * (toDegree(std::atan(std::pow(10.,mp/(a * std::log(10))))) - 45);

    // the original formula is the fa less some value, so the real
    // solution will be greater than fa
    const auto d = 1;
    auto fb = fa + d > 90 ? 90 : fa + d;
    while (mp > Latitude(fb).getMeridionalPart(ref)) {
        fa = fb;
        fb += d;
    }

    // bisection method
    auto oldSol = fa < 90. ? fa : 90.;
    auto sol = (fb + fa) / 2. < 90. ? (fb + fa) / 2. : 90.;
    while(std::abs(sol - oldSol) > tol) {
        oldSol = sol;
        if (Latitude(sol).getMeridionalPart(ref) < mp){
            fa = sol;
        } else {
            fb = sol;
        }
        sol = (fa + fb) / 2.;
    }
    sol = ntl::utils::round(sol, 5);
    if (sign) {
        return ntl::Latitude(sol);
    }
    return ntl::Latitude(-sol);
}

double ntl::Latitude::getMeridionalPartInRadians(
    const Ellipsoid &ref
) const noexcept {
    using namespace utils;
    const auto e = ref.getEccentricity();
    const auto latRad = utils::toRadians(value);
    const double a = std::tan(
        M_PI / 4. +
        latRad / 2.
    );
    const double b = std::pow(
        (1 - e * std::sin(latRad)) / (1 + e * std::sin(latRad)),
        e / 2.
    );

    return std::log(a * b);
}

ntl::Latitude ntl::Latitude::operator-() const {
    return Latitude(-this->value);
}

double ntl::Latitude::getGeocentricValueInRadians(
    const ntl::Ellipsoid &ref
) const noexcept {
    return std::atan(
        (1-std::pow(ref.getEccentricity(), 2)) *
        std::tan(utils::toRadians(value))
    );
}

double ntl::Latitude::getGeocentricValue(
    const ntl::Ellipsoid &ref
) const noexcept {
    return utils::toDegree(getGeocentricValueInRadians(ref));
}

std::ostream &::ntl::operator<<(
    ostream &out, const ntl::Latitude &obj
) {
    return (out << fmt::format(
        "{}˚ {}' {}'' {}",
        obj.getDeg(), obj.getMin(), obj.getSec(), obj.getSign()
    ));
}
