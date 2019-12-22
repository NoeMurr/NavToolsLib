#include <utility>

//
// Created by Noè Murr on 2018-11-28.
//

#include <navtools/GES.hpp>
#include <navtools/utils/mathUtils.hpp>
#include <cmath>

using namespace ntl;

struct vec3 {
    double x,y,z;
};

struct GEP {
    double lambda, mu;
};

static inline double getGeocentric(const Latitude &l, double e) {
    const auto rad = utils::toRadians(l.getValue());
    return std::atan((1-std::pow(e,2)) * std::tan(rad));
}

static inline vec3 getCartesianCoord(const Point &p, const Ellipsoid &ref) {
    const auto latRad = utils::toRadians(p.latitude.getValue());
    const auto longRad = utils::toRadians(p.longitude.getValue());
    const double R = ref.getSemiMajorAxis() /
                      std::sqrt(
                          1. - std::pow(ref.getEccentricity(), 2) *
                               std::pow(std::sin(latRad), 2)
                      );
    return {
        R * std::cos(latRad) * std::cos(longRad), // x
        R * std::cos(latRad) * std::sin(longRad), // y
        R * (1. - std::pow(ref.getEccentricity(), 2.)) * std::sin(latRad)
    };
}

// returns the Great Elliptic Plan parameters lambda and mu of the equation
// z=z=lambda x+mu y that defines the the Great Elliptic Plan
static inline GEP getGEPParams(
    const Point &a, const Point &b, const Ellipsoid &ref
){
    const auto cartA = getCartesianCoord(a, ref);
    const auto cartB = getCartesianCoord(b, ref);
    return {
        (cartA.y * cartB.z - cartB.y * cartA.z) /
        (cartA.y * cartB.x - cartB.y * cartA.x), // lambda

        (cartA.x * cartB.z - cartB.x * cartA.z) /
        (cartA.x * cartB.y - cartB.x * cartA.y) // mu
    };
}

static inline double  calcGepEccentricity(
    const Point &vertex, const Ellipsoid &ref
){
    const auto theta = getGeocentric(vertex.latitude, ref.getEccentricity());
    return  (ref.getEccentricity() * std::sin(theta)) /
            std::sqrt(
                1. - std::pow(ref.getEccentricity(), 2.) *
                     std::pow(std::cos(theta),2.)
            );
}

static inline Point calcVertexCoord(
    const Point &a,
    const Point &b,
    const Ellipsoid &ref
) {

    const auto gep = getGEPParams(a, b, ref);

    const auto lambda = std::atan(gep.mu/gep.lambda);
    const auto beta = std::atan(std::sqrt(
        (std::pow(gep.lambda,2.) + std::pow(gep.mu, 2.)) /
        (1.-std::pow(ref.getEccentricity(),2.))
    ));
    const auto theta = std::acos(
        std::cos(beta) /
        std::sqrt(
            1.-std::pow(ref.getEccentricity(),2.) *
            std::pow(std::sin(beta),2.)
        )
    );

    const auto phi = std::atan(
        std::tan(theta) /
        (1. -std::pow(ref.getEccentricity(),2.))
    );

    return Point{
        Latitude(utils::toDegree(phi)), Longitude(utils::toDegree(lambda))
    };
}

static inline std::pair<Point, Point> calcEquatorIntersections(
    const Point &vertex
){
    return {
        Point{
            Latitude(),
            Longitude(vertex.longitude.getValue() - 90.)
        },
        Point{
            Latitude(),
            Longitude(-(vertex.longitude.getValue() - 90.))
        }
    };
}

static inline double calcDistance(
    const Point &dep,
    const Point &arr,
    const Point &vertex,
    const Ellipsoid &ref
) {
    const double e = calcGepEccentricity(vertex, ref);
    // intersection between equator and the GEP
    const double lambdaE = vertex.longitude.getValue() - 90.;

    double thetaGEA = std::acos(
        std::cos(std::abs(getGeocentric(dep.latitude, ref.getEccentricity()))) *
        std::cos(std::abs(
            utils::toRadians(dep.longitude.getValue() - lambdaE)
        ))
    );

    double thetaGEB = std::acos(
        std::cos(getGeocentric(arr.latitude, ref.getEccentricity())) *
        std::cos(
            utils::toRadians(arr.longitude.getValue() - lambdaE)
        )
    );

    if (not dep.latitude.isSign()) thetaGEA = -thetaGEA;
    if (not arr.latitude.isSign()) thetaGEB = -thetaGEB;

    double phiGEA = std::atan(std::tan(thetaGEA)/(1-std::pow(e, 2.)));
    double phiGEB = std::atan(std::tan(thetaGEB)/(1-std::pow(e, 2.)));

    if (std::abs(thetaGEA) > M_PI/2.) {
        if (thetaGEA < 0){
           phiGEA = -(M_PI - std::abs(phiGEA));
        } else {
           phiGEA = M_PI - std::abs(phiGEA);
        }
    }
    if (std::abs(thetaGEB) > M_PI/2.) {
        if (thetaGEB < 0){
            phiGEB = -(M_PI - std::abs(phiGEB));
        } else {
            phiGEB = M_PI - std::abs(phiGEB);
        }
    }

    const double M[5] = {
        1. + (3./4.)*std::pow(e,2) + (45./64.)*std::pow(e,4) +
        (175./256.)*std::pow(e,6) + (11025./16384.)*std::pow(e, 8), // M0

        (3./8.)*std::pow(e,2) + (15./32.)*std::pow(e,4) +
        (525./1024.)*std::pow(e,6) + (2205./4096.)*std::pow(e, 8),// M2

        (15./256.)*std::pow(e,4) + (105./1024.)*std::pow(e,6) +
        (2205./8820.)*std::pow(e, 8),// M4

        (35./3072.)*std::pow(e,6) + (315./12288.)*std::pow(e, 8), // M6

        (315./130784.)*std::pow(e, 8) // M8
    };

    return std::abs(
        ref.getSemiMajorAxis() * (1-std::pow(e,2)) *
        (
            M[0] * (phiGEB - phiGEA) -
            M[1] * (std::sin(2*phiGEB) - std::sin(2*phiGEA)) +
            M[2] * (std::sin(4*phiGEB) - std::sin(4*phiGEA)) -
            M[3] * (std::sin(6*phiGEB) - std::sin(6*phiGEA)) +
            M[4] * (std::sin(8*phiGEB) - std::sin(8*phiGEA))
        )
    );
}

static inline Course calcCourse(
    const Point &dep,
    const Point &arr,
    const Ellipsoid &ref
) {
    auto phiA = utils::toRadians(dep.latitude.getValue());
    auto thetaA = getGeocentric(dep.latitude, ref.getEccentricity());
    auto thetaB = getGeocentric(arr.latitude, ref.getEccentricity());
    auto deltaLambda = utils::toRadians( // difference of longitude.
        arr.longitude.getValue() - dep.longitude.getValue()
    );
    // if d_l > 180 d_l = 360 - d_l
    if (std::abs(deltaLambda) > M_PI) {
        deltaLambda = deltaLambda < 0 ? 2 * M_PI - std::abs(deltaLambda)
                                      : -(2 * M_PI - std::abs(deltaLambda));
    }

    auto Ri = std::atan(
        std::sin(deltaLambda) /
        (
            std::cos(thetaA) * std::tan(thetaB) -
            std::sin(thetaA) * std::cos(deltaLambda)
        )
    );

    Ri = std::atan(std::tan(Ri) * std::cos(phiA - thetaA));

    if (Ri < 0) Ri += M_PI;
    if (deltaLambda < 0) Ri += M_PI;

    return Course(utils::toDegree(Ri));
}

GES::GES(
    Point departure,
    Point arrival,
    Ellipsoid reference
) : reference(std::move(reference)),
    departure(std::move(departure)),
    arrival(std::move(arrival)),
    vertex(calcVertexCoord(this->departure, this->arrival, this->reference)),
    equatorIntersections(calcEquatorIntersections(vertex)),
    eccentricity(calcGepEccentricity(vertex, reference)),
    distance(calcDistance(
        this->departure, this->arrival, vertex, this->reference
    )),
    course(calcCourse(this->departure, this->arrival, this->reference))
{
    // Empty...
}

const Point &GES::getDeparture() const {
    return departure;
}

const Point &GES::getArrival() const {
    return arrival;
}

const Course &GES::getCourse() const {
    return course;
}

const Ellipsoid &GES::getReference() const {
    return reference;
}

double GES::getDistance() const {
    return distance;
}

const Point &GES::getVertex() const {
    return vertex;
}

double GES::getEccentricity() const {
    return eccentricity;
}

const pair<Point, Point> &GES::getEquatorIntersections() const {
    return equatorIntersections;
}
