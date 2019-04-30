#include <navtools/Point.hpp>
#include <sstream>
#include <catch.hpp>

TEST_CASE("Point Creation and print", "[point]") {
    ntl::Point a(ntl::Latitude{0}, ntl::Longitude{0});
    ntl::Point b(ntl::Latitude{45, 16, 34, 'S'}, ntl::Longitude{90, 56, 33, 'W'});

    ostringstream ss;
    ss << a;
    REQUIRE("{Lat: 0˚ 0' 0'' N, Long: 0˚ 0' 0'' E}" == ss.str());

    ss.str("");
    ss << b;
    REQUIRE("{Lat: 45˚ 16' 34'' S, Long: 90˚ 56' 33'' W}" == ss.str());
}

TEST_CASE("Point Comparations", "[point]") {
    ntl::Point a(ntl::Latitude(0), ntl::Longitude(0));
    ntl::Point b(ntl::Latitude{45, 16, 34, 'S'}, ntl::Longitude{90, 56, 33, 'W'});
    ntl::Point c(ntl::Latitude{0, 0, 0, 'S'}, ntl::Longitude{0});

    REQUIRE(a != b);
    REQUIRE(a == c);
    REQUIRE(b != c);
    REQUIRE(a == a);
    REQUIRE(b == b);
    REQUIRE(c == c);
}