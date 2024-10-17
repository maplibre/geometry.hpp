#include <catch2/catch_all.hpp>
#include <maplibre/geometry.hpp>

using maplibre::geometry::empty;
using maplibre::geometry::geometry;
using maplibre::geometry::geometry_collection;
using maplibre::geometry::line_string;
using maplibre::geometry::multi_line_string;
using maplibre::geometry::multi_point;
using maplibre::geometry::multi_polygon;
using maplibre::geometry::point;
using maplibre::geometry::polygon;

TEST_CASE("test empty in geometry")
{
    geometry<int64_t> g0;
    CHECK(std::holds_alternative<empty>(g0));

    empty n1;
    geometry<int64_t> g1(n1);
    CHECK(std::holds_alternative<empty>(g1));

    CHECK(g0 == g1);
}

TEST_CASE("test point in geometry")
{
    point<int64_t> p1(1, 2);
    geometry<int64_t> g1(p1);
    CHECK(std::holds_alternative<point<int64_t>>(g1));

    geometry<int64_t> g2(p1);
    CHECK(std::holds_alternative<point<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test multi point in geometry")
{
    multi_point<int64_t> mp1;
    mp1.emplace_back(1, 2);

    geometry<int64_t> g1(mp1);
    CHECK(std::holds_alternative<multi_point<int64_t>>(g1));

    geometry<int64_t> g2(mp1);
    CHECK(std::holds_alternative<multi_point<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test line string in geometry")
{
    line_string<int64_t> ls1;
    ls1.emplace_back(1, 2);
    ls1.emplace_back(3, 4);

    geometry<int64_t> g1(ls1);
    CHECK(std::holds_alternative<line_string<int64_t>>(g1));

    geometry<int64_t> g2(ls1);
    CHECK(std::holds_alternative<line_string<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test multi line string in geometry")
{
    multi_line_string<int64_t> mls1 = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};

    geometry<int64_t> g1(mls1);
    CHECK(std::holds_alternative<multi_line_string<int64_t>>(g1));

    geometry<int64_t> g2(mls1);
    CHECK(std::holds_alternative<multi_line_string<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test polygon in geometry")
{
    polygon<int64_t> poly1 = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};

    geometry<int64_t> g1(poly1);
    CHECK(std::holds_alternative<polygon<int64_t>>(g1));

    geometry<int64_t> g2(poly1);
    CHECK(std::holds_alternative<polygon<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test multi polygon in geometry")
{
    multi_polygon<int64_t> mp1 = {{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}};

    geometry<int64_t> g1(mp1);
    CHECK(std::holds_alternative<multi_polygon<int64_t>>(g1));

    geometry<int64_t> g2(mp1);
    CHECK(std::holds_alternative<multi_polygon<int64_t>>(g2));

    CHECK(g1 == g2);
}

TEST_CASE("test geometry")
{
    geometry<int64_t> eg; // default constructed geometry is empty
    CHECK(std::holds_alternative<empty>(eg));

    geometry<int64_t> pg{point<int64_t>()};
    CHECK(std::holds_alternative<point<int64_t>>(pg));

    geometry<int64_t> lsg{line_string<int64_t>()};
    CHECK(std::holds_alternative<line_string<int64_t>>(lsg));

    geometry<int64_t> pgg{polygon<int64_t>()};
    CHECK(std::holds_alternative<polygon<int64_t>>(pgg));

    geometry<int64_t> mpg{multi_point<int64_t>()};
    CHECK(std::holds_alternative<multi_point<int64_t>>(mpg));

    geometry<int64_t> mlsg{multi_line_string<int64_t>()};
    CHECK(std::holds_alternative<multi_line_string<int64_t>>(mlsg));

    geometry<int64_t> mpgg{multi_polygon<int64_t>()};
    CHECK(std::holds_alternative<multi_polygon<int64_t>>(mpgg));

    geometry<int64_t> gcg{geometry_collection<int64_t>()};
    CHECK(std::holds_alternative<geometry_collection<int64_t>>(gcg));

    CHECK(pg == pg);
    CHECK(!(pg != pg));
    CHECK(pg != lsg);
}

TEST_CASE("test geometry collection")
{
    geometry_collection<int64_t> gc1;
    CHECK(gc1.empty());

    CHECK(gc1 == gc1);
    CHECK(!(gc1 != gc1));
}
