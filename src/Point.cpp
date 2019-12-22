//
// Created by Noè Murr on 28/08/2017.
//

#include <navtools/Point.hpp>
#include <utility>

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
    ntl::Latitude latitude,
    ntl::Longitude longitude
) : latitude(std::move(latitude)), longitude(std::move(longitude)) {}


