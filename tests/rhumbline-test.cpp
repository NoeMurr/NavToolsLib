#include <navtools/rhumbline.hpp>
#include <navtools/utils/mathUtils.hpp>
#include <sstream>
#include <cmath>
#include <catch.hpp>

static bool rhumbCheck(const ntl::Rhumbline &lhs, const ntl::Rhumbline &rhs) {

    if (lhs.getReference() != rhs.getReference()) return false;
    if (lhs.getDistance() != rhs.getDistance()) return false;
    if (lhs.getCourse() != rhs.getCourse()) return false;

    auto lA = lhs.getDeparture();
    auto rA = rhs.getDeparture();
    auto lB = lhs.getArrival();
    auto rB = rhs.getArrival();

    if (lA.latitude.getDeg() != rA.latitude.getDeg() or
        lA.latitude.getMin() != rA.latitude.getMin() or
        std::round(lA.latitude.getSec()) != std::round(lA.latitude.getSec())) {
        return false;
    }

    if (lA.longitude.getDeg() != rA.longitude.getDeg() or
        lA.longitude.getMin() != rA.longitude.getMin() or
        std::round(lA.longitude.getSec()) !=
        std::round(lA.longitude.getSec())) {
        return false;
    }

    if (lB.latitude.getDeg() != rB.latitude.getDeg() or
        lB.latitude.getMin() != rB.latitude.getMin() or
        std::round(lB.latitude.getSec()) != std::round(lB.latitude.getSec())) {
        return false;
    }

    return !(lB.longitude.getDeg() != rB.longitude.getDeg() or
             lB.longitude.getMin() != rB.longitude.getMin() or
             std::round(lB.longitude.getSec()) !=
             std::round(lB.longitude.getSec()));
}

TEST_CASE("rhumbline calculation", "[rhumbline]") {
    // NE
    ntl::Point a(ntl::Latitude(3, 51, 36, 'N'), ntl::Longitude(5, 56, 54, 'E'));
    ntl::Point b(ntl::Latitude(84, 12, 7, 'N'), ntl::Longitude(52, 2, 38, 'E'));
    // SE
    ntl::Point c(ntl::Latitude(44, 21, 58, 'S'), ntl::Longitude(174, 35, 12, 'E'));
    ntl::Point d(ntl::Latitude(69, 9, 50, 'S'), ntl::Longitude(58, 33, 43, 'E'));
    // NW
    ntl::Point e(ntl::Latitude(57, 35, 46, 'N'), ntl::Longitude(63, 55, 23, 'W'));
    ntl::Point f(ntl::Latitude(81, 30, 27, 'N'), ntl::Longitude(56, 35, 2, 'W'));
    // SW
    ntl::Point g(ntl::Latitude(40, 16, 2, 'S'), ntl::Longitude(17, 29, 4, 'W'));
    ntl::Point h(ntl::Latitude(29, 52, 42, 'S'), ntl::Longitude(13, 28, 51, 'W'));

    // meridian
    ntl::Point i(ntl::Latitude(44, 14, 9, 'N'), ntl::Longitude(161, 50, 10, 'W'));
    ntl::Point j(ntl::Latitude(85, 9, 11, 'S'), ntl::Longitude(161, 50, 10, 'W'));

    // parallel
    ntl::Point k(ntl::Latitude(66, 22, 57, 'N'), ntl::Longitude(179, 24, 19, 'E'));
    ntl::Point l(ntl::Latitude(66, 22, 57, 'N'), ntl::Longitude(4, 1, 44, 'E'));

    // equator
    ntl::Point m(ntl::Latitude(), ntl::Longitude(6, 39, 9, 'W'));
    ntl::Point n(ntl::Latitude(), ntl::Longitude(53, 6, 18, 'E'));

    // tests
    ntl::Rhumbline *tmp = nullptr;
    ntl::Rhumbline ij(i, j);
    ntl::Rhumbline kl(k, l);
    ntl::Rhumbline mn(m, n);

    ntl::Rhumbline ab(a, b);
    ntl::Rhumbline bc(b, c);
    ntl::Rhumbline ag(a, g);
    ntl::Rhumbline be(b, e);

    ntl::Rhumbline cd(c, d);
    ntl::Rhumbline db(d, b);
    ntl::Rhumbline cf(c, f);
    ntl::Rhumbline dh(d, h);

    ntl::Rhumbline ef(e, f);
    ntl::Rhumbline fa(f, a);
    ntl::Rhumbline ec(e, c);
    ntl::Rhumbline fg(f, g);

    ntl::Rhumbline gh(g, h);
    ntl::Rhumbline hb(h, b);
    ntl::Rhumbline gd(g, d);
    ntl::Rhumbline he(h, e);

    CHECK(ntl::utils::round(ij.getCourse().getValue(), 1) == 180.);
    CHECK(ntl::utils::round((ij.getDistance() / 1852.), 1) == 7754.1);
    tmp = new ntl::Rhumbline(i, ij.getCourse(), ij.getDistance());
    if (!rhumbCheck(ij, *tmp)) {
        FAIL_CHECK("ij != tmp" << ij << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(kl.getCourse().getValue(), 1) == 270.);
    CHECK(ntl::utils::round((kl.getDistance() / 1852.), 1) == 4223.2);
    tmp = new ntl::Rhumbline(k, kl.getCourse(), kl.getDistance());
    if (!rhumbCheck(kl, *tmp)) {
        FAIL_CHECK("kl != tmp\n" << kl << " == " << *tmp);
    }
    delete tmp;

    CHECK(mn.getCourse().getValue() == 90);
    CHECK(ntl::utils::round((mn.getDistance() / 1852.), 1) == 3591.9);
    tmp = new ntl::Rhumbline(m, mn.getCourse(), mn.getDistance());
    if (!rhumbCheck(mn, *tmp)) {
        FAIL_CHECK("mn != tmp\n" << mn << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(ab.getCourse().getValue(), 1) == 15.5);
    CHECK(ntl::utils::round((ab.getDistance() / 1852.), 1) == 5001.4);
    tmp = new ntl::Rhumbline(a, ab.getCourse(), ab.getDistance());
    if (!rhumbCheck(ab, *tmp)) {
        FAIL_CHECK("ab != tmp\n" << ab << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(bc.getCourse().getValue(), 1) == 150.9);
    CHECK(ntl::utils::round((bc.getDistance() / 1852.), 1) == 8820.4);
    tmp = new ntl::Rhumbline(b, bc.getCourse(), bc.getDistance());
    if (!rhumbCheck(bc, *tmp)) {
        FAIL_CHECK("bc != tmp\n" << bc << " == " << *tmp);
    }
    delete tmp;

    //a --> g: c: 206.2   nm: 2950.5
    CHECK(ntl::utils::round(ag.getCourse().getValue(), 1) == 206.2);
    CHECK(ntl::utils::round((ag.getDistance() / 1852.), 1) == 2940.0);
    tmp = new ntl::Rhumbline(a, ag.getCourse(), ag.getDistance());
    if (!rhumbCheck(ag, *tmp)) {
        FAIL_CHECK("ag != tmp\n" << ag << " == " << *tmp);
    }
    delete tmp;

    //b --> e: c: 229.2   nm: 2443.9
    CHECK(ntl::utils::round(be.getCourse().getValue(), 1) == 229.2);
    CHECK(ntl::utils::round((be.getDistance() / 1852.), 1) == 2453.5);
    tmp = new ntl::Rhumbline(b, be.getCourse(), be.getDistance());
    if (!rhumbCheck(be, *tmp)) {
        FAIL_CHECK("be != tmp\n" << be << " == " << *tmp);
    }
    delete tmp;

    // c --> d: c: 247.8   nm: 3938.7
    CHECK(ntl::utils::round(cd.getCourse().getValue(), 1) == 247.8);
    CHECK(ntl::utils::round((cd.getDistance() / 1852.), 1) == 3946.8);
    tmp = new ntl::Rhumbline(c, cd.getCourse(), cd.getDistance());
    if (!rhumbCheck(cd, *tmp)) {
        FAIL_CHECK("cd != tmp\n" << cd << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(db.getCourse().getValue(), 1) == 358.6);
    CHECK(ntl::utils::round((db.getDistance() / 1852.), 1) == 9198.3);
    tmp = new ntl::Rhumbline(d, db.getCourse(), db.getDistance());
    if (!rhumbCheck(db, *tmp)) {
        FAIL_CHECK("db != tmp\n" << db << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(cf.getCourse().getValue(), 1) == 33.1);
    CHECK(ntl::utils::round((cf.getDistance() / 1852.), 1) == 8998.8);
    tmp = new ntl::Rhumbline(c, cf.getCourse(), cf.getDistance());
    if (!rhumbCheck(cf, *tmp)) {
        FAIL_CHECK("cf != tmp\n" << cf << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(dh.getCourse().getValue(), 1) == 312.3);
    CHECK(ntl::utils::round((dh.getDistance() / 1852.), 1) == 3505.9);
    tmp = new ntl::Rhumbline(d, dh.getCourse(), dh.getDistance());
    if (!rhumbCheck(dh, *tmp)) {
        FAIL_CHECK("dh != tmp\n" << dh << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(ef.getCourse().getValue(), 1) == 5.4);
    CHECK(ntl::utils::round((ef.getDistance() / 1852.), 1) == 1446.5);
    tmp = new ntl::Rhumbline(e, ef.getCourse(), ef.getDistance());
    if (!rhumbCheck(ef, *tmp)) {
        FAIL_CHECK("ef != tmp\n" << ef << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(fa.getCourse().getValue(), 1) == 156.6);
    CHECK(ntl::utils::round((fa.getDistance() / 1852.), 1) == 5074.0);
    tmp = new ntl::Rhumbline(f, fa.getCourse(), fa.getDistance());
    if (!rhumbCheck(fa, *tmp)) {
        FAIL_CHECK("fa != tmp\n" << fa << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(ec.getCourse().getValue(), 1) == 225.4);
    CHECK(ntl::utils::round((ec.getDistance() / 1852.), 1) == 8689.5);
    tmp = new ntl::Rhumbline(e, ec.getCourse(), ec.getDistance());
    if (!rhumbCheck(ec, *tmp)) {
        FAIL_CHECK("ec != tmp\n" << ec << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(fg.getCourse().getValue(), 1) == 168.5);
    CHECK(ntl::utils::round((fg.getDistance() / 1852.), 1) == 7445.4);
    tmp = new ntl::Rhumbline(f, fg.getCourse(), fg.getDistance());
    if (!rhumbCheck(fg, *tmp)) {
        FAIL_CHECK("fg != tmp\n" << fg << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(gh.getCourse().getValue(), 1) == 17.5);
    CHECK(ntl::utils::round((gh.getDistance() / 1852.), 1) == 652.7);
    tmp = new ntl::Rhumbline(g, gh.getCourse(), gh.getDistance());
    if (!rhumbCheck(gh, *tmp)) {
        FAIL_CHECK("gh != tmp\n" << gh << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(hb.getCourse().getValue(), 1) == 18.);
    CHECK(ntl::utils::round((hb.getDistance() / 1852.), 1) == 7188.2);
    tmp = new ntl::Rhumbline(h, hb.getCourse(), hb.getDistance());
    if (!rhumbCheck(hb, *tmp)) {
        FAIL_CHECK("hb != tmp\n" << hb << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(gd.getCourse().getValue(), 1) == 124.8);
    CHECK(ntl::utils::round((gd.getDistance() / 1852.), 1) == 3042.8);
    tmp = new ntl::Rhumbline(g, gd.getCourse(), gd.getDistance());
    if (!rhumbCheck(gd, *tmp)) {
        FAIL_CHECK("gd != tmp\n" << gd << " == " << *tmp);
    }
    delete tmp;

    CHECK(ntl::utils::round(he.getCourse().getValue(), 1) == 333.6);
    CHECK(ntl::utils::round((he.getDistance() / 1852.), 1) == 5843.0);
    tmp = new ntl::Rhumbline(h, he.getCourse(), he.getDistance());
    if (!rhumbCheck(he, *tmp)) {
        FAIL_CHECK("he != tmp\n" << he << " == " << *tmp);
    }
    delete tmp;
}

TEST_CASE("paper rhumblines", "[rhumbline]") {
    using namespace ntl::utils;
    const ntl::Point Points[]{
        // New Port Boston 0
        ntl::Point{ntl::Latitude( 41.1056), ntl::Longitude(-71.391)},
        // Leath Harbour 1
        ntl::Point{ntl::Latitude(-54.1317), ntl::Longitude(-36.538)},
        // Buenos Ayres 2
        ntl::Point{ntl::Latitude(-36.0627),   ntl::Longitude(-55.504)},
        // Dakar 3
        ntl::Point{ntl::Latitude( 14.3691),   ntl::Longitude(-17.454)},
        // Pearth Australia 4
        ntl::Point{ntl::Latitude(-32.1036),   ntl::Longitude( 115.568)},
        //Mombasa 5
        ntl::Point{ntl::Latitude(-4.0987),    ntl::Longitude( 39.711)},
        // San Francisco 6
        ntl::Point{ntl::Latitude( 37.7508),   ntl::Longitude(-122.700)},
        // Yoko Hama, Japan 7
        ntl::Point{ntl::Latitude( 34.4363),   ntl::Longitude( 139.852)},
        // Valparaiso 8
        ntl::Point{ntl::Latitude(-33.0000),   ntl::Longitude(-71.611)},
        // Sydney 9
        ntl::Point{ntl::Latitude(-33.7702),   ntl::Longitude( 151.533)},
        // Cape of Good Hope 10
        ntl::Point{ntl::Latitude(-34.4268),   ntl::Longitude( 18.432)},
        // Rio de Janeiro 11
        ntl::Point{ntl::Latitude(-23.0319),   ntl::Longitude(-43.120)},
        // Beirut 12
        ntl::Point{ntl::Latitude( 33.9336),   ntl::Longitude( 35.505)},
        // Tobruk 13
        ntl::Point{ntl::Latitude( 32.0641),   ntl::Longitude( 24.011)},
        // Lisbon 14
        ntl::Point{ntl::Latitude( 38.6201),   ntl::Longitude( -9.223)}
    };

    const std::pair<double, double> results[] { // routes and distances
        {111.3, 4598.2},
        {295.6, 9315.84},
        {267.6, 4741.36},
        {132.2, 6724.67},
        {193.6, 4216.77},
        { 88.5, 3634.6},
        { 93.0, 2875.55},
        {101.9, 3309.24},
        {259.1, 590.68},
        { 89.6, 6875.63},
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
        ntl::Rhumbline rhumb(Points[routes[i].first], Points[routes[i].second]);
        INFO(
            "from: " << routes[i].first << " " << Points[routes[i].first] <<
            " to: "  << routes[i].second << " " << Points[routes[i].second]
        )
        double tolerance = 0.1;
        if (results[i].second > 2000){
            tolerance = 0.5;
        } else if (results[i].second > 500) {
            tolerance = 0.3;
        } else if (results[i].second > 250) {
            tolerance = 0.2;
        }
        CHECK(round(rhumb.getCourse().getValue(), 1) == results[i].first);
        CHECK(
            std::abs(
                round((rhumb.getDistance() / 1852.), 2) - results[i].second
            ) < tolerance
        );
    }
}

TEST_CASE("ECDIS rhumblines", "[rhumbline]") {
    using namespace ntl::utils;
    const ntl::Point Points[]{
        // New Port Boston 0
        ntl::Point{ntl::Latitude( 41.1056), ntl::Longitude(-71.391)},
        // Leath Harbour 1
        ntl::Point{ntl::Latitude(-54.1317), ntl::Longitude(-36.538)},
        // Buenos Ayres 2
        ntl::Point{ntl::Latitude(-36.0627),   ntl::Longitude(-55.504)},
        // Dakar 3
        ntl::Point{ntl::Latitude( 14.3691),   ntl::Longitude(-17.454)},
        // Pearth Australia 4
        ntl::Point{ntl::Latitude(-32.1036),   ntl::Longitude( 115.568)},
        //Mombasa 5
        ntl::Point{ntl::Latitude(-4.0987),    ntl::Longitude( 39.711)},
        // San Francisco 6
        ntl::Point{ntl::Latitude( 37.7508),   ntl::Longitude(-122.700)},
        // Yoko Hama, Japan 7
        ntl::Point{ntl::Latitude( 34.4363),   ntl::Longitude( 139.852)},
        // Valparaiso 8
        ntl::Point{ntl::Latitude(-33.0000),   ntl::Longitude(-71.611)},
        // Sydney 9
        ntl::Point{ntl::Latitude(-33.7702),   ntl::Longitude( 151.533)},
        // Cape of Good Hope 10
        ntl::Point{ntl::Latitude(-34.4268),   ntl::Longitude( 18.432)},
        // Rio de Janeiro 11
        ntl::Point{ntl::Latitude(-23.0319),   ntl::Longitude(-43.120)},
        // Beirut 12
        ntl::Point{ntl::Latitude( 33.9336),   ntl::Longitude( 35.505)},
        // Tobruk 13
        ntl::Point{ntl::Latitude( 32.0641),   ntl::Longitude( 24.011)},
        // Lisbon 14
        ntl::Point{ntl::Latitude( 38.6201),   ntl::Longitude( -9.223)}
    };

    const std::pair<double, double> results[] { // routes and distances
        {111.3, 4598.21},
        {295.6, 9315.98},
        {267.6, 4741.41},
        {132.2, 6724.75},
        { 88.5, 3634.64},
        {101.9, 3309.28},
        {193.6, 4216.8},
        { 93.0, 2875.56},
        {259.1, 590.67},
        { 89.6, 6875.8},
    };

    const std::pair<unsigned int, unsigned int> routes[] = {
        {5,     4},
        {8,     7},
        {6,     7},
        {0,     10},
        {2,     10},
        {11,    10},
        {3,     1},
        {0,     14},
        {12,    13},
        {9,     8},
    };

    for (int i = 0; i < 10; ++i) {
        ntl::Rhumbline rhumb(Points[routes[i].first], Points[routes[i].second]);
        INFO(
            "from: " << routes[i].first << " " << Points[routes[i].first] <<
            " to: "  << routes[i].second << " " << Points[routes[i].second]
        )
        double tolerance = 0.1;
        if (results[i].second > 2000){
            tolerance = 0.5;
        } else if (results[i].second > 500) {
            tolerance = 0.3;
        } else if (results[i].second > 250) {
            tolerance = 0.2;
        }
        CHECK(round(rhumb.getCourse().getValue(), 1) == results[i].first);
        CHECK(
            std::abs(
                round((rhumb.getDistance() / 1852.), 2) - results[i].second
            ) < tolerance
        );
    }
}