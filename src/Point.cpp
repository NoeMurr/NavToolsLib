//
// Created by Noè Murr on 28/08/2017.
//

#include "../hdr/navtools/Point.hpp"

std::ostream &ntl::operator<<(std::ostream &out, const Point &obj) {
    out << "{Lat: " << obj.latitude << ", Long: " << obj.longitude << "}";
    return out;
}

bool ntl::Point::operator==(const ntl::Point &o) const {
    return latitude == o.latitude and longitude == o.longitude;
}

bool ntl::Point::operator!=(const ntl::Point &o) const {
    return not(*this == o);
}

ntl::Point::Point(
    const ntl::Latitude &latitude,
    const ntl::Longitude &longitude
) : latitude(latitude), longitude(longitude) {}


