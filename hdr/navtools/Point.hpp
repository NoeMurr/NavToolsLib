//
// Created by Noè Murr on 28/08/2017.
//

#ifndef NAVTOOLS_POINT_HPP
#define NAVTOOLS_POINT_HPP

#include "Latitude.hpp"
#include "Longitude.hpp"


namespace ntl {
    struct Point {
        Latitude latitude;
        Longitude longitude;

        explicit Point(
            const Latitude &latitude    = Latitude(),
            const Longitude &longitude  = Longitude()
        );

        bool operator==(const Point &o) const;

        bool operator!=(const Point &o) const;

    };

    std::ostream &operator<<(std::ostream &out, const Point &obj);
}


#endif //NAVTOOLS_POINT_HPP
