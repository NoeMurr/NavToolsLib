//
// Created by Noè Murr on 27/07/2018.
//

#ifndef NAVTOOLS_COURSE_HPP
#define NAVTOOLS_COURSE_HPP

#include <ostream>

namespace ntl {
    class Course {
        double value;

        struct Relative {
            friend class Course; // course can create Relative object
            const char prefix;
            const int deg;
            const int min;
            const double sec;
            const char suffix;

        private:
            Relative(
                char prefix, int deg, int min, double sec, char suffix
            );

        public:
            bool operator==(const Relative &o) const;

            bool operator!=(const Relative &o) const;

            double getNumericValue() const;
        };

        struct Absolute {
            friend class Course; // only Course can create objects of type
            // Absolute
            const int deg;
            const int min;
            const double sec;

        private:
            Absolute(int deg, int min, double sec);

        public:
            bool operator==(const Absolute &o) const;

            bool operator!=(const Absolute &o) const;

            double getNumericValue() const;
        };

    public:
        using Circular = Absolute;
        using SemiCircular = Relative;
        using Quadrantal =  Relative;

        explicit Course(double value = 0) noexcept(false);

        void setValue(double value) noexcept(false);

        double getValue() const;

        Circular getCircular() const;

        Course::SemiCircular getSemiCircular(char prefix) const;

        Quadrantal getQuadrantal() const;

        bool operator==(const Course &rhs) const;

        bool operator!=(const Course &rhs) const;

        friend std::ostream &
        operator<<(std::ostream &os, const Course &course);

        friend std::ostream &
        operator<<(std::ostream &os, const Relative &relative);

        friend std::ostream &
        operator<<(std::ostream &os, const Absolute &circular);
    };

    std::ostream &
    operator<<(std::ostream &os, const Course &course);

    std::ostream &
    operator<<(std::ostream &os, const Course::Relative &relative);

    std::ostream &
    operator<<(std::ostream &os, const Course::Absolute &circular);
}


#endif //NAVTOOLS_COURSE_HPP
