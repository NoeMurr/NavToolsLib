#include <navtools/utils/mathUtils.hpp>
#include <navtools/Latitude.hpp>
#include <sstream>
#include <catch.hpp>
#include <fmt/format.h>

static double
meridionalDistance(const ntl::Latitude &lat, const ntl::Ellipsoid &ref) {
    const double n =
            (ref.getSemiMajorAxis() - ref.getSemiMinorAxis()) /
            (ref.getSemiMajorAxis() + ref.getSemiMinorAxis());

    const double a =
            (ref.getSemiMajorAxis() * pow(1. + (1. / 8) * pow(n, 2), 2)) / (1 + n);

    const double b =
            1. - (3. / 8) * pow(n, 2);

    const double radLat = lat.getValue();

    return a * (radLat - b * (3. / 2) * n * sin(2 * radLat) -
                (15. / 16) * pow(n, 2) * sin(4 * radLat) +
                (35. / 48) * pow(n, 3) * sin(6 * radLat) -
                (315. / 512) * pow(n, 4) * sin(8 * radLat));

}

TEST_CASE("Latitude Creation and print", "[latitude]") {
    ntl::Latitude a;
    ntl::Latitude b(70, 60, 60);
    ntl::Latitude c(70, 1);
    ntl::Latitude d(70, 1, 3);
    ntl::Latitude e(70, 1, 3, 'S');
    ntl::Latitude f;
    ntl::Latitude *g = nullptr;
    ntl::Latitude h;
    f.setValue(45.);
    ostringstream os;

    REQUIRE_THROWS(g = new ntl::Latitude(89, 60, 1));
    REQUIRE(g == nullptr);

    // copy constructor and assignment
    h = ntl::Latitude(e);
    REQUIRE(h == e);

    os << a;
    REQUIRE("0˚ 0' 0'' N" == os.str());

    os.str("");
    os << b;
    REQUIRE("71˚ 1' 0'' N" == os.str());

    os.str("");
    os << c;
    REQUIRE("70˚ 1' 0'' N" == os.str());

    os.str("");
    os << d;
    REQUIRE("70˚ 1' 3'' N" == os.str());

    os.str("");
    os << e;
    REQUIRE("70˚ 1' 3'' S" == os.str());

    os.str("");
    os << f;
    REQUIRE("45˚ 0' 0'' N" == os.str());
}

TEST_CASE("Meridional part check", "[latitude]") {
    ntl::Latitude a;
    ntl::Latitude b(10);
    ntl::Latitude c(10, 30);
    ntl::Latitude d(20);
    ntl::Latitude e(20, 30);
    ntl::Latitude f(30);
    ntl::Latitude g(30, 30);
    ntl::Latitude h(40);
    ntl::Latitude i(40, 30);
    ntl::Latitude j(50);
    ntl::Latitude k(50, 30);
    ntl::Latitude l(60);
    ntl::Latitude m(60, 30);
    ntl::Latitude n(70);
    ntl::Latitude o(70, 30);
    ntl::Latitude p(80);
    ntl::Latitude q(80, 30);

    CHECK(ntl::utils::round(a.getMeridionalPart(), 1) == 0);
    CHECK(ntl::utils::round(b.getMeridionalPart(), 1) == 599.1);
    CHECK(ntl::utils::round(c.getMeridionalPart(), 1) == 629.4);
    CHECK(ntl::utils::round(d.getMeridionalPart(), 1) == 1217.3);
    CHECK(ntl::utils::round(e.getMeridionalPart(), 1) == 1249.1);
    CHECK(ntl::utils::round(f.getMeridionalPart(), 1) == 1876.9);
    CHECK(ntl::utils::round(g.getMeridionalPart(), 1) == 1911.4);
    CHECK(ntl::utils::round(h.getMeridionalPart(), 1) == 2607.9);
    CHECK(ntl::utils::round(i.getMeridionalPart(), 1) == 2647.0);
    CHECK(ntl::utils::round(j.getMeridionalPart(), 1) == 3456.8);
    CHECK(ntl::utils::round(k.getMeridionalPart(), 1) == 3503.6);
    CHECK(ntl::utils::round(l.getMeridionalPart(), 1) == 4507.4);
    CHECK(ntl::utils::round(m.getMeridionalPart(), 1) == 4567.8);
    CHECK(ntl::utils::round(n.getMeridionalPart(), 1) == 5944.2);
    CHECK(ntl::utils::round(o.getMeridionalPart(), 1) == 6033.0);
    CHECK(ntl::utils::round(p.getMeridionalPart(), 1) == 8352.5);
    CHECK(ntl::utils::round(q.getMeridionalPart(), 1) == 8529.6);
}

TEST_CASE("Latitude Comparations", "[latitude]") {
    ntl::Latitude a;
    ntl::Latitude b(10);
    ntl::Latitude c(10, 30);
    ntl::Latitude d(20);
    ntl::Latitude e(20, 30);
    ntl::Latitude f(30);
    ntl::Latitude g(30, 30);
    ntl::Latitude h(40);
    ntl::Latitude i(40, 30, 0, 'S');
    ntl::Latitude j(50, 0, 0, 'S');
    ntl::Latitude k(50, 30, 0, 'S');
    ntl::Latitude l(60, 0, 0, 'S');
    ntl::Latitude m(60, 30, 0, 'S');
    ntl::Latitude n(70, 0, 0, 'S');
    ntl::Latitude o(70, 30, 0, 'S');
    ntl::Latitude p(80, 0, 0, 'S');

    ntl::Latitude q(10);
    ntl::Latitude r(40, 30, 0, 'S');
    ntl::Latitude s(20, 30);
    ntl::Latitude t(80, 0, 0, 'S');

    // positives
    REQUIRE((a < b && b > a && a <= b && b >= a));
    REQUIRE((c < d && d > c && c <= d && d >= c));
    REQUIRE((e < f && f > e && e <= f && f >= e));
    REQUIRE((g < h && h > g && g <= h && h >= g));

    // negatives
    REQUIRE((i > j && j < i && i >= j && j <= i));
    REQUIRE((k > l && l < k && k >= l && l <= k));
    REQUIRE((m > n && n < m && m >= n && n <= m));
    REQUIRE((o > p && p < o && o >= p && p <= o));

    // both
    REQUIRE((p < a && a > p && p <= a && a >= p));

    // equality
    REQUIRE((q == b && r == i && s == e && t == p));
    REQUIRE((q >= b && r >= i && s >= e && t >= p));
    REQUIRE((q <= b && r <= i && s <= e && t <= p));

    // inequality
    REQUIRE((a != b && b != c && a != c));
    REQUIRE((a != ntl::Latitude(-b.getValue())));
}