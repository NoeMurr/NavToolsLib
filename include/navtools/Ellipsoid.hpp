#include <utility>

//
// Created by Noè Murr on 19/08/2018.
//

#ifndef NAVTOOLS_ELLIPSOID_HPP
#define NAVTOOLS_ELLIPSOID_HPP

#include <cmath>
#include <string>
#include <ostream>
#include <vector>

namespace ntl {
    struct Ellipsoid {
    private:
        static const std::vector<Ellipsoid> standards;

    public:
        static const Ellipsoid &getStd(const std::string &name);

    private:
        double semiMajorAxis;
        double semiMinorAxis;
        double flattering;
        double eccentricity;
        std::string name;

    public:
        Ellipsoid(
            double semiMajorAxis, double semiMinorAxis,
            double flattering, std::string name = ""
        );

        friend std::ostream &
        operator<<(std::ostream &os, const Ellipsoid &ref);

        bool operator==(const Ellipsoid &rhs) const;

        bool operator!=(const Ellipsoid &rhs) const;

        double getSemiMajorAxis() const;

        double getSemiMinorAxis() const;

        double getFlattering() const;

        double getEccentricity() const;

        const std::string &getName() const;

    };

    std::ostream &operator<<(std::ostream &os, const Ellipsoid &ref);
}


#endif //NAVTOOLS_ELLIPSOID_HPP
