//
// Created by Noè Murr on 28/08/2017.
//

#ifndef NAVTOOLS_LONGITUDE_HPP
#define NAVTOOLS_LONGITUDE_HPP

#include "DegreeValue.hpp"

using namespace std;

namespace ntl {
    class Longitude : public DegreeValue{
    public:
        explicit Longitude(
            int deg = 0, int min = 0, double sec = 0,
            char sign = 'E'
        ) noexcept(false);

        explicit Longitude(double value);

        bool operator==(const Longitude &o) const;

        bool operator!=(const Longitude &o) const;

        bool operator>(const Longitude &o) const;

        bool operator<(const Longitude &o) const;

        bool operator<=(const Longitude &o) const;

        bool operator>=(const Longitude &o) const;

        Longitude operator-() const;

        void setSign(char sign) noexcept;
        char getSign() const noexcept ;

    };

    std::ostream &operator<<(ostream &out, const Longitude &obj);

}


#endif //NAVTOOLS_LONGITUDE_HPP
