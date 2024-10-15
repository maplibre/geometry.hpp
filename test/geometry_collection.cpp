#include <catch2/catch_all.hpp>
#include <maplibre/geometry.hpp>

TEST_CASE("test geometry collection with out geometry compiles correctly")
{
    maplibre::geometry::geometry_collection<double> gc;
    CHECK(gc.empty());
}
