//
// Created by Noè Murr on 29/08/2017.
//

#include "../hdr/navtools/rhumbline.hpp"
#include "../hdr/navtools/utils/mathUtils.hpp"
#include "../hdr/navtools/Ellipsoid.hpp"
#include "../hdr/navtools/Longitude.hpp"
#include <cmath>
#include <sstream>
#include <fmt/ostream.h>

using namespace ntl;

static double meridionalDistance(const Latitude &lat, const Ellipsoid &ref) {
    const double n =
        (ref.getSemiMajorAxis() - ref.getSemiMinorAxis()) /
        (ref.getSemiMajorAxis() + ref.getSemiMinorAxis());

    const double a =
        (ref.getSemiMajorAxis() * std::pow(1. + (1. / 8) * std::pow(n, 2.), 2.)) /
        (1 + n);

    const double b =
        1. - (3. / 8) * std::pow(n, 2);

    const double radLat = utils::toRadians(lat.getValue());

    return a *  (radLat - b * (3. / 2.) * n * std::sin(2 * radLat) -
                (15. / 16.) * std::pow(n, 2.) * std::sin(4 * radLat) +
                (35. / 48.) * std::pow(n, 3.) * std::sin(6 * radLat) -
                (315. / 512.) * std::pow(n, 4.) * std::sin(8 * radLat));
}

static Longitude arrivalLongitude(
    Point departure, Course course, double distance,
    const Latitude &arrivalLat, const Ellipsoid &ref, bool round = true
) {
    using namespace ntl::utils;

    double arrivalLong;
    Course::Quadrantal qCourse = course.getQuadrantal();
    double radiansRq = toRadians(qCourse.getNumericValue());
    double diffMeridPart = ntl::utils::round(
        abs(arrivalLat.getMeridionalPart(ref) -
        departure.latitude.getMeridionalPart(ref)), 5
    );
    double diffLong;

    if (course.getValue() == 0 or
        course.getValue() == 180) {
        diffLong = 0;
    } else if (course.getValue() == 90 or
               course.getValue() == 270) {
        auto lat = std::abs(toRadians(departure.latitude.getValue()));
        diffLong = toDegree(distance / (ref.getSemiMajorAxis() * std::cos(lat)));
    } else {
        diffLong = (diffMeridPart * tan(radiansRq)) / 60;
    }

    if (qCourse.suffix == 'W')
        diffLong *= -1;

    arrivalLong = departure.longitude.getValue() + diffLong;

    if (round) {
        auto s = arrivalLong > 0;
        if (abs(arrivalLong) > 180) {
            arrivalLong = 360 - abs(arrivalLong);
            if (s) arrivalLong *= -1;
        }
    }

    return Longitude(arrivalLong);
}

static Course courseBetween(
    Point departure, Point arrival, const Ellipsoid &ref
) {
    // precision only up to 5º digits after dot.
    using namespace ntl::utils;
    Course course;
    auto diffLat =
        toRadians(arrival.latitude.getValue()) -
        toRadians(departure.latitude.getValue());
    auto diffLong =
        toRadians(arrival.longitude.getValue()) -
        toRadians(departure.longitude.getValue());
    auto diffMeridPart =
        arrival.latitude.getMeridionalPartInRadians(ref) -
        departure.latitude.getMeridionalPartInRadians(ref);


    char prefix = diffLat > 0 ? 'N' : 'S';
    char suffix = diffLong > 0 ? 'E' : 'W';

    diffLong = abs(diffLong);
    diffMeridPart = abs(diffMeridPart);
    diffLat = abs(diffLat);


    if (diffLat == 0 and diffLong == 0) {
        course.setValue(0);
    } else if (diffLat == 0) {
        if (suffix == 'W')
            course.setValue(270);
        else
            course.setValue(90);
    } else if (diffLong == 0) {
        if (prefix == 'S')
            course.setValue(180);
        else
            course.setValue(0);
    } else {
        if (diffLong > M_PI) {
            diffLong = 2 * M_PI - diffLong;
            suffix = suffix == 'E' ? 'W' : 'E';
        }

        double courseValue = toDegree(atan(diffLong / diffMeridPart));

        if (prefix == 'S' and suffix == 'E') {
            courseValue = 180 - courseValue;
        } else if (prefix == 'S' and suffix == 'W') {
            courseValue = 180 + courseValue;
        } else if (prefix == 'N' and suffix == 'W') {
            courseValue = 360 - courseValue;
        }

        course.setValue(courseValue);
    }

    return course;
}

/** returns distance in meters **/
static double distanceBetween(
    const Point &a, const Point &b, const Course &course, const Ellipsoid &ref
) {

    if (course.getCircular().getNumericValue() == 90 or
        course.getCircular().getNumericValue() == 270) {
        auto lat = utils::toRadians(a.latitude.getValue());
        auto longB = utils::toRadians(
            b.longitude.getValue()
        );
        auto longA = utils::toRadians(
            a.longitude.getValue()
        );

        return ref.getSemiMajorAxis() * std::abs(longB - longA) * std::cos(lat);
    }

    auto c = utils::toRadians(
        course.getCircular().getNumericValue()
    );

    auto dm = meridionalDistance(b.latitude, ref) -
              meridionalDistance(a.latitude, ref);
    return dm/std::cos(c);
}

/** Expects distance in meters **/
Latitude arrivalLatitude(
    const Latitude &departureLat, const Course &c, double distance,
    const Ellipsoid &ref
) {

    auto m1 = meridionalDistance(departureLat, ref);
    auto c1 = c.getCircular().getNumericValue();
    auto dm = distance * cos(ntl::utils::toRadians(c1));

    auto m2 = m1 + dm;
    auto l2 = m2 / 1852.;

    double old = m2, sol = 0;

    while (abs(old - sol) > (1852. * 0.1 /* tolerance */)) {
        old = sol;
        sol = meridionalDistance(Latitude{l2 / 60.}, ref);
        l2 = l2 + (m2 - sol) / 1852.;
    }

    return Latitude{l2 / 60.};
}

std::ostream &ntl::operator<<(std::ostream &os, const Rhumbline &rhm) {
    return (os << "from: " << rhm.departure << " to: " << rhm.arrival
               << "\n\tcourse: " << rhm.course.getCircular() << " distance: "
               << rhm.distance << "\nReference ellipsoid: " << rhm.reference);
}

const Point &Rhumbline::getDeparture() const { return departure; }

const Point &Rhumbline::getArrival() const { return arrival; }

const Course &Rhumbline::getCourse() const { return course; }

double Rhumbline::getDistance() const { return distance; }

const Ellipsoid &Rhumbline::getReference() const {
    return reference;
}

Rhumbline::Rhumbline(
    const Point &departure, const Course &course,
    double distance, const Ellipsoid &reference
) : reference(reference), departure(departure), course(course),
    distance(distance) {

    auto arrivalLat = arrivalLatitude(
        departure.latitude, course, distance,
        reference
    );
    auto arrivalLong = arrivalLongitude(
        departure, course, distance, arrivalLat,
        reference
    );
    arrival = Point(arrivalLat, arrivalLong);
}

Rhumbline::Rhumbline(
    const Point &departure, const Point &arrival, const Ellipsoid &reference
) : reference(reference), departure(departure), arrival(arrival) {

    course = courseBetween(departure, arrival, reference);
    distance = distanceBetween(departure, arrival, course, reference);
}

bool Rhumbline::operator==(const Rhumbline &rhs) const {
    return departure == rhs.departure &&
           arrival == rhs.arrival &&
           course == rhs.course &&
           reference == rhs.reference &&
           distance == rhs.distance;
}

bool Rhumbline::operator!=(const Rhumbline &rhs) const {
    return !(rhs == *this);
}

bool Rhumbline::isInPath(const Latitude &parallel) const noexcept {
    return  parallel > std::min(departure.latitude, arrival.latitude) and
            parallel < std::max(departure.latitude, arrival.latitude);
}

bool Rhumbline::isInPath(const Longitude &meridian) const noexcept {
    return  meridian > std::min(departure.longitude, arrival.longitude) and
            meridian < std::max(departure.longitude, arrival.longitude);
}

Point Rhumbline::getIntersection(
    const Latitude &parallel
) const noexcept(false) {
    // the main formula is dLambda/dMP = tanRq
    // todo test

    if (not isInPath(parallel))
        // the parallel specified is out of the rhumbline path
        throw NoIntersectionError(*this, parallel);

    const double tanRq = std::tan(
        utils::toRadians(course.getQuadrantal().getNumericValue())
    );
    const double dMP = std::abs(
        parallel.getMeridionalPart(reference) -
        departure.latitude.getMeridionalPart(reference)
    );
    double dLambda = tanRq * dMP;

    if (dLambda == 0) {
        return Point(parallel, departure.longitude);
    }

    if (course.getQuadrantal().suffix == 'W') dLambda *= -1;

    const Longitude intersectionLong(
        departure.longitude.getValue() + dLambda / 60.
    );

    if (not isInPath(intersectionLong))
        // the specified meridian is out of the rhumbline
        throw NoIntersectionError(*this, parallel);

    return Point(parallel, intersectionLong);
}

Point Rhumbline::getIntersection(
    const Longitude &meridian
) const noexcept(false){
    if (not isInPath(meridian))
        // the specified meridian is out of the rhumbline
        throw NoIntersectionError(*this, meridian);

    const double tanRq   = std::tan(
        utils::toRadians(course.getQuadrantal().getNumericValue())
    );
    const double dLambda = std::abs(
        meridian.getValue() - departure.longitude.getValue()
    ) * 60.;
    double dMP     = dLambda / tanRq;
    if (course.getQuadrantal().prefix == 'S') dMP *= -1;

    const auto intersectionLat = Latitude::approxFromMeridionalPart(
        departure.latitude.getMeridionalPart(reference) + dMP,
        reference
    );

    if (not isInPath(intersectionLat))
        // no intersection has been found between departure and arrival
        throw NoIntersectionError(*this, meridian);

    return Point(intersectionLat, meridian);
}

Rhumbline::Rhumbline(const Rhumbline &oth)
    : departure(oth.departure), arrival(oth.arrival), course(oth.course),
      reference(oth.reference), distance(oth.distance){}


Rhumbline::NoIntersectionError::NoIntersectionError(
    const Rhumbline &rhumb,
    const Latitude &latitude
) : logic_error(
    fmt::format(
        "cannot find an intersection between rhumbline: {} and parallel: {}",
        rhumb,
        latitude
    )
){}

Rhumbline::NoIntersectionError::NoIntersectionError(
    const Rhumbline &rhumb,
    const Longitude &longitude
) : logic_error(
    fmt::format(
        "cannot find an intersection between rhumbline: {} and meridian: {}",
        rhumb,
        longitude
    )
){}
