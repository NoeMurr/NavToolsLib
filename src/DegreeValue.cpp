//
// Created by Noè Murr on 28/08/2017.
//

#include <navtools/DegreeValue.hpp>
#include <navtools/utils/mathUtils.hpp>
#include <iomanip>
#include <fmt/format.h>

using namespace std;
namespace ut = ntl::utils;

int ntl::DegreeValue::getDeg() const noexcept{
    return static_cast<int>(std::abs(value));
}

void ntl::DegreeValue::setDeg(unsigned int deg) noexcept(false) {
    double v = (std::abs(value) - getDeg()) + deg;

    setValue(value < 0 ? -v : v);
}

int ntl::DegreeValue::getMin() const noexcept{
    return static_cast<int>(ut::round((std::abs(value) - getDeg()) * 60, 8));
}

void ntl::DegreeValue::setMin(unsigned int min) noexcept(false){
    auto v = (std::abs(value) - (getMin()/60.)) + (min/60.);

    setValue(value < 0 ? -v : v);
}

double ntl::DegreeValue::getSec() const noexcept{
    auto sec = (((std::abs(value) - getDeg()) * 60.) - getMin()) * 60.;
    return ntl::utils::round(std::abs(sec), 8);
}

void ntl::DegreeValue::setSec(double sec) noexcept(false){
    sec = ut::round(sec, 8);
    auto v = (std::abs(value) - (getSec()/3600.)) + (sec/3600.);

    setValue(value < 0 ? -v : v);
}

bool ntl::DegreeValue::isSign() const noexcept{
    return value >= 0;
}

void ntl::DegreeValue::setSign(bool sign) noexcept{
    if (sign)
        value = std::abs(value);
    else
        value = -std::abs(value);
}

int ntl::DegreeValue::getMaxValue() const noexcept{
    return maxValue;
}

ntl::DegreeValue::DegreeValue(
        const int maxValue, int deg, int min, double sec, bool sign
) noexcept(false) : value(0.), maxValue(maxValue) {
    setValue(
        sign ? (deg + (min / 60.) + (sec / 3600.))
             : -(deg + (min / 60.) + (sec / 3600.))
    );
}

double ntl::DegreeValue::getValue() const noexcept {
    return value;
}

void ntl::DegreeValue::setValue(double v) noexcept(false) {
    if (std::abs(v) > maxValue) {
        throw invalid_argument(
            fmt::format(
                "this degree value must be less than {0} and greater than -{0}",
                maxValue
            )
        );
    }
    this->value = v;
}

ntl::DegreeValue::DegreeValue(
        int maxValue, double value
) : value(value), maxValue(maxValue) {}

std::ostream &::ntl::operator<<(
        std::ostream &out, const ntl::DegreeValue &obj
) {
    return (out << fmt::format(
        "Degree{ deg = {}, min = {}, sec = %.2f, sign = {}, maxValue = {} }",
        obj.getDeg(), obj.getMin(), obj.getSec(), 
        obj.isSign() ? "true" : "false", obj.getMaxValue()
    ));
}
