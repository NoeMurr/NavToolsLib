//
// Created by Noè Murr on 28/08/2017.
//

#include "../hdr/navtools/Longitude.hpp"
#include <fmt/format.h>

ntl::Longitude::Longitude(
    int deg, int min, double sec, char sign
) noexcept(false) : DegreeValue(180, deg, min, sec, sign == 'E') {}

bool ntl::Longitude::operator==(
    const ntl::Longitude &o
) const {
    return value == o.value;
}

bool ntl::Longitude::operator!=(
    const ntl::Longitude &o
) const {
    return !(*this == o);
}

bool ntl::Longitude::operator>(
    const ntl::Longitude &o
) const {
    return value > o.value;
}

bool ntl::Longitude::operator<(
    const ntl::Longitude &o
) const {
    return value < o.value;
}

bool ntl::Longitude::operator<=(
    const ntl::Longitude &o
) const {
    return value <= o.value;
}

bool ntl::Longitude::operator>=(
    const ntl::Longitude &o
) const {
    return value >= o.value;
}

void ntl::Longitude::setSign(char sign) noexcept {
    DegreeValue::setSign(sign == 'E');
}

char ntl::Longitude::getSign() const noexcept{
    return isSign() ? 'E' : 'W';
}

ntl::Longitude::Longitude(double value) : DegreeValue(180, value) {}

ntl::Longitude ntl::Longitude::operator-() const {
    return Longitude(-value);
}

std::ostream & ntl::operator<<(ostream &out, const Longitude &obj) {
    return (out << fmt::format(
        "{}˚ {}' {}'' {}",
        obj.getDeg(), obj.getMin(), obj.getSec(), obj.getSign()
    ));
}
