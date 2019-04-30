//
// Created by Noè Murr on 2018-11-28.
//

#ifndef NAVTOOLS_GES_HPP
#define NAVTOOLS_GES_HPP

#include "Point.hpp"
#include "Course.hpp"
#include <utility>

namespace ntl {
    /**
     * @brief this class represents a GreatEllipticSailing path.
     */
    class GES {

    private:
        Point departure;
        Point arrival;
        Ellipsoid reference;
        Point vertex;
        std::pair<Point, Point> equatorIntersections;
        double eccentricity;
        double distance;
        Course course;

    public:
        GES(
            ntl::Point departure,
            ntl::Point arrival,
            ntl::Ellipsoid reference =
                ntl::Ellipsoid::getStd("WGS-84")
        );

        const Point &getDeparture() const;

        const Point &getArrival() const;

        const Course &getCourse() const;

        const Ellipsoid &getReference() const;

        double getDistance() const;

        const Point &getVertex() const;

        double getEccentricity() const;

        const pair<Point, Point> &getEquatorIntersections() const;
    };
}


#endif //NAVTOOLS_GES_HPP
