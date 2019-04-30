//
// Created by Noè Murr on 28/08/2017.
//

#ifndef NAVTOOLS_LATITUDE_HPP
#define NAVTOOLS_LATITUDE_HPP

#include <iostream>
#include "DegreeValue.hpp"
#include "Ellipsoid.hpp"

using namespace std;

namespace ntl {
    class Latitude : public DegreeValue{
    public:
        explicit Latitude(
            int deg = 0, int min = 0, double sec = 0,
            char sign = 'N'
        ) noexcept(false);

        explicit Latitude(double value) noexcept(false);

        bool operator==(const Latitude &o) const;

        bool operator!=(const Latitude &o) const;

        bool operator>(const Latitude &o) const;

        bool operator<(const Latitude &o) const;

        bool operator<=(const Latitude &o) const;

        bool operator>=(const Latitude &o) const;

        Latitude operator-() const;

        char getSign() const noexcept;
        void setSign(char sign) noexcept;

        double getMeridionalPart(
            const Ellipsoid &ref = Ellipsoid::getStd("WGS-84")
        ) const noexcept;

        double getMeridionalPartInRadians(
            const Ellipsoid &ref = Ellipsoid::getStd("WGS-84")
        ) const noexcept;

        double getGeocentricValue(
            const Ellipsoid &ref = Ellipsoid::getStd("WGS-84")
        ) const noexcept;

        double getGeocentricValueInRadians(
            const Ellipsoid &ref = Ellipsoid::getStd("WGS-84")
        ) const noexcept;

    public: // static
        static Latitude approxFromMeridionalPart(
            double mp,
            const Ellipsoid &ref = Ellipsoid::getStd("WGS-84")
        );
    };

    std::ostream &operator<<(ostream &out, const Latitude &obj);

}

#endif //NAVTOOLS_LATITUDE_HPP
