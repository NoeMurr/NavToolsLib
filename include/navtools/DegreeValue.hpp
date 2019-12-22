//
// Created by Noè Murr on 28/08/2017.
//

#ifndef NAVTOOLS_DEGREEVALUE_HPP
#define NAVTOOLS_DEGREEVALUE_HPP

#include <stdexcept>

namespace ntl {
    /**
     *
     */
    class DegreeValue {
    public:

        explicit DegreeValue(
            int maxValue, int deg = 0, int min = 0, double sec = 0.,
            bool sign = true
        ) noexcept(false);

        DegreeValue(int maxValue, double value) noexcept(false);

        virtual int getDeg() const noexcept;

        virtual void setDeg(unsigned int deg) noexcept(false);

        virtual int getMin() const noexcept;

        virtual void setMin(unsigned int min) noexcept(false);

        virtual double getSec() const noexcept;

        virtual void setSec(double sec) noexcept(false);

        virtual bool isSign() const noexcept;

        virtual void setSign(bool sign) noexcept;

        virtual int getMaxValue() const noexcept;

        virtual double getValue() const noexcept;

        virtual void setValue(double v) noexcept(false);

    protected:
        double value;
    private:
        int maxValue;
    };

    std::ostream &operator<<(std::ostream &out, const DegreeValue &obj);
}

#endif //NAVTOOLS_DEGREEVALUE_HPP
