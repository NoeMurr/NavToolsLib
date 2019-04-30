//
// Created by Noè Murr on 27/07/2018.
//


#include <stdexcept>
#include "../hdr/navtools/Course.hpp"
#include "../hdr/navtools/utils/mathUtils.hpp"

using namespace ntl;

Course::Course(double value) noexcept(false) {
    setValue(value);
}

bool Course::operator==(const Course &rhs) const {
    return value == rhs.value;
}

bool Course::operator!=(const Course &rhs) const {
    return !(rhs == *this);
}

std::ostream &
ntl::operator<<(std::ostream &os, const Course &course) {
    os << "value: " << course.value;
    return os;
}

void Course::setValue(double value) noexcept(false) {
    while (value < 0.) value = 360 + value;
    if (value > 360.)
        throw std::invalid_argument("Course value must be less then 360");
    this->value = value;
}

Course::Relative::Relative(
    char prefix, int deg, int min, double sec, char suffix
) : prefix(prefix), deg(deg), min(min), sec(sec), suffix(suffix) {}

bool Course::Relative::operator==(const Course::Relative &o) const {
    return prefix == o.prefix and deg == o.deg and min == o.min and
           sec == o.sec and suffix == o.suffix;
}

bool Course::Relative::operator!=(const Course::Relative &o) const {
    return !(o == *this);
}

double Course::Relative::getNumericValue() const {
    return deg + (min / 60.) + (sec / 3600);
}

std::ostream &
ntl::operator<<(std::ostream &os, const Course::Relative &relative) {
    os << relative.prefix << " " << relative.deg
       << "˚ " << relative.min << "' " << relative.sec
       << "'' " << relative.suffix;
    return os;
}

std::ostream &
ntl::operator<<(std::ostream &os, const Course::Absolute &circular) {
    os << circular.deg << "˚ " << circular.min << "' " << circular.sec << "'' ";
    return os;
}

Course::Quadrantal Course::getQuadrantal() const {
    char prefix, suffix;
    double value;
    int deg, min;
    double sec;

    if (this->value < 90) {
        prefix = 'N';
        suffix = 'E';
        value = this->value;
    } else if (this->value < 180) {
        prefix = 'S';
        suffix = 'E';
        value = 180 - this->value;
    } else if (this->value < 270) {
        prefix = 'S';
        suffix = 'W';
        value = this->value - 180;
    } else {
        prefix = 'N';
        suffix = 'W';
        value = 360 - this->value;
    }

    deg = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    min = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    sec = value;

    return {prefix, deg, min, sec, suffix};
}

Course::SemiCircular Course::getSemiCircular(char prefix) const {
    double value;
    char suffix;
    int deg, min;
    double sec;

    if (prefix == 'N') {
        if (this->value < 180) {
            value = this->value;
            suffix = 'E';
        } else {
            value = 360 - this->value;
            suffix = 'W';
        }
    } else if (prefix == 'S') {
        if (this->value < 180) {
            value = 180 - this->value;
            suffix = 'E';
        } else {
            value = this->value - 180;
            suffix = 'W';
        }
    } else {
        throw std::invalid_argument("invalid prefix to getSemiCircularCourse");
    }


    deg = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    min = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    sec = value;

    return {prefix, deg, min, sec, suffix};
}

Course::Circular Course::getCircular() const {
    auto value = this->value;
    int deg;
    int min;
    double sec;

    deg = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    min = (static_cast<int>(value));
    value = (value - static_cast<int>(value)) * 60;
    sec = value;

    return {deg, min, sec};
}

double Course::getValue() const {
    return value;
}

Course::Absolute::Absolute(int deg, int min, double sec)
        : deg(deg), min(min), sec(sec) {}

bool Course::Absolute::operator==(const Course::Absolute &o) const {
    return o.deg == deg and o.min == min and o.sec == sec;
}

bool Course::Absolute::operator!=(const Course::Absolute &o) const {
    return !(o == *this);
}

double Course::Absolute::getNumericValue() const {
    return deg + (min / 60.) + (sec / 3600);
}