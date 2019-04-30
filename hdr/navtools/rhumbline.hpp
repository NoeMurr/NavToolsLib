//
// Created by Noè Murr on 29/08/2017.
//

#ifndef NAVTOOLS_RHUMBLINE_HPP
#define NAVTOOLS_RHUMBLINE_HPP

#include <ostream>
#include "Point.hpp"
#include "Course.hpp"

namespace ntl {
    class Rhumbline {
        Point departure;
        Point arrival;
        Course course;
        Ellipsoid reference;
        double distance;


        friend std::ostream &operator<<(
            std::ostream &os, const Rhumbline &rhumbline
        );

    public:
        Rhumbline(
            const Point &departure, const Course &course,
            double distance,
            const Ellipsoid &reference = Ellipsoid::getStd("WGS-84")
        );

        explicit Rhumbline(
            const Point &departure= Point(), const Point &arrival = Point(),
            const Ellipsoid &reference = Ellipsoid::getStd("WGS-84")
        );

        Rhumbline(const Rhumbline &oth);

        Rhumbline &operator=(const Rhumbline &oth) = default;

    public:
        const Point &getDeparture() const;

        const Point &getArrival() const;

        const Course &getCourse() const;

        double getDistance() const;

        const Ellipsoid &getReference() const;

        bool isInPath(const Latitude &parallel) const noexcept;

        bool isInPath(const Longitude &meridian) const noexcept;

        Point getIntersection(const Latitude &parallel) const noexcept(false);

        Point getIntersection(const Longitude &meridian) const noexcept(false);

    public:
        bool operator==(const Rhumbline &rhs) const;

        bool operator!=(const Rhumbline &rhs) const;

    public: // exceptions
        struct NoIntersectionError : public std::logic_error{

            NoIntersectionError(
                const Rhumbline &rhumb,
                const Latitude &latitude
            );

            NoIntersectionError(
                const Rhumbline &rhumb,
                const Longitude &longitude
            );
        };
    };

    ostream &operator<<(ostream &os, const Rhumbline &rhumbline);
}


#endif //NAVTOOLS_RHUMBLINE_HPP
