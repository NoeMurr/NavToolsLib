#include <navtools/Longitude.hpp>
#include <sstream>
#include <catch.hpp>

TEST_CASE("Longitude Creation and print", "[longitude]") {
    ntl::Longitude a;
    ntl::Longitude b(70, 60, 60);
    ntl::Longitude c(70, 1);
    ntl::Longitude d(70, 1, 3);
    ntl::Longitude e(170, 1, 3, 'W');
    ntl::Longitude f;
    ntl::Longitude *g = nullptr;
    ntl::Longitude h;
    f.setValue(90.);
    ostringstream os;

    REQUIRE_THROWS(g = new ntl::Longitude(179, 60, 1));
    REQUIRE(g == nullptr);

    h = ntl::Longitude(e);
    REQUIRE(h == e);

    os << a;
    REQUIRE("0˚ 0' 0'' E" == os.str());

    os.str("");
    os << b;
    REQUIRE("71˚ 1' 0'' E" == os.str());

    os.str("");
    os << c;
    REQUIRE("70˚ 1' 0'' E" == os.str());

    os.str("");
    os << d;
    REQUIRE("70˚ 1' 3'' E" == os.str());

    os.str("");
    os << e;
    REQUIRE("170˚ 1' 3'' W" == os.str());

    os.str("");
    os << f;
    REQUIRE("90˚ 0' 0'' E" == os.str());
}

TEST_CASE("Longitude Comparations", "[longitude]") {
    ntl::Longitude a;
    ntl::Longitude b(20);
    ntl::Longitude c(20, 30);
    ntl::Longitude d(40);
    ntl::Longitude e(40, 30);
    ntl::Longitude f(60);
    ntl::Longitude g(60, 30);
    ntl::Longitude h(80);
    ntl::Longitude i(80, 30, 0, 'W');
    ntl::Longitude j(90, 0, 0, 'W');
    ntl::Longitude k(90, 30, 0, 'W');
    ntl::Longitude l(120, 0, 0, 'W');
    ntl::Longitude m(120, 30, 0, 'W');
    ntl::Longitude n(140, 0, 0, 'W');
    ntl::Longitude o(140, 30, 0, 'W');
    ntl::Longitude p(160, 0, 0, 'W');

    ntl::Longitude q(20);
    ntl::Longitude r(80, 30, 0, 'W');
    ntl::Longitude s(40, 30);
    ntl::Longitude t(160, 0, 0, 'W');

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
    REQUIRE((a != ntl::Longitude(-b.getValue())));
}