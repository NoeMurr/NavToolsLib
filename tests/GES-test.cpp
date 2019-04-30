//
// Created by Noè Murr on 2019-03-01.
//
#include <navtools/GES.hpp>
#include <navtools/utils/mathUtils.hpp>
#include <sstream>
#include <cmath>
#include <catch.hpp>
#include <navtools/Point.hpp>

TEST_CASE("GES", "[GES]") {
    using namespace ntl::utils;
    const ntl::Point points[]{
        // New Port Boston 0
        ntl::Point{ntl::Latitude(41.1056), ntl::Longitude(-71.391)},
        // Leath Harbour 1
        ntl::Point{ntl::Latitude(-54.1317), ntl::Longitude(-36.538)},
        // Buenos Ayres 2
        ntl::Point{ntl::Latitude(-36.0627), ntl::Longitude(-55.504)},
        // Dakar 3
        ntl::Point{ntl::Latitude(14.3691), ntl::Longitude(-17.454)},
        // Pearth Australia 4
        ntl::Point{ntl::Latitude(-32.1036), ntl::Longitude(115.568)},
        //Mombasa 5
        ntl::Point{ntl::Latitude(-4.0987), ntl::Longitude(39.711)},
        // San Francisco 6
        ntl::Point{ntl::Latitude(37.7508), ntl::Longitude(-122.700)},
        // Yoko Hama, Japan 7
        ntl::Point{ntl::Latitude(34.4363), ntl::Longitude(139.852)},
        // Valparaiso 8
        ntl::Point{ntl::Latitude(-33.0000), ntl::Longitude(-71.611)},
        // Sydney 9
        ntl::Point{ntl::Latitude(-33.7702), ntl::Longitude(151.533)},
        // Cape of Good Hope 10
        ntl::Point{ntl::Latitude(-34.4268), ntl::Longitude(18.432)},
        // Rio de Janeiro 11
        ntl::Point{ntl::Latitude(-23.0319), ntl::Longitude(-43.120)},
        // Beirut 12
        ntl::Point{ntl::Latitude(33.9336), ntl::Longitude(35.505)},
        // Tobruk 13
        ntl::Point{ntl::Latitude(32.0641), ntl::Longitude(24.011)},
        // Lisbon 14
        ntl::Point{ntl::Latitude(38.6201), ntl::Longitude(-9.223)}
    };

    const double distance_results[] = {
        4557.23,
        9242.78,
        4502.86,
        6699.26,
        4213.54,
        3541.08,
        2813.74,
        3268.78,
         590.39,
        6128.95
    };

    const double course_results[] = {
        121.9,
        281.9,
        302.0,
        117.3,
        191.8,
        112.1,
         71.6,
        116.7,
        262.2,
        144.0
    };

    const std::pair<unsigned int, unsigned int> routes[] = {
        {5,     4},
        {8,     7},
        {6,     7},
        {0,     10},
        {3,     1},
        {2,     10},
        {0,     14},
        {11,    10},
        {12,    13},
        {9,     8},
    };

    for (int i = 0; i < 10; ++i) {
        ntl::GES ges(points[routes[i].first], points[routes[i].second]);
        INFO(
            "from: " << routes[i].first << " " << points[routes[i].first] <<
            " to: "  << routes[i].second << " " << points[routes[i].second]
        )
        double tolerance = 0.1;
        if (distance_results[i] > 2000){
            tolerance = 0.5;
        } else if (distance_results[i] > 500) {
            tolerance = 0.3;
        } else if (distance_results[i] > 250) {
            tolerance = 0.2;
        }
        CHECK(
            std::abs(
                round((ges.getDistance() / 1852.), 2) - distance_results[i]
            ) < tolerance
        );
        CHECK(
            ntl::utils::round(ges.getCourse().getCircular().getNumericValue(),1)
            == course_results[i]
        );
    }
}