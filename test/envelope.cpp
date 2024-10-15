#include <catch2/catch_all.hpp>
#include <maplibre/geometry/envelope.hpp>

using maplibre::geometry::box;
using maplibre::geometry::envelope;
using maplibre::geometry::geometry;
using maplibre::geometry::geometry_collection;
using maplibre::geometry::line_string;
using maplibre::geometry::multi_line_string;
using maplibre::geometry::multi_point;
using maplibre::geometry::multi_polygon;
using maplibre::geometry::point;
using maplibre::geometry::polygon;

TEST_CASE("test envelope")
{
    CHECK(envelope(point<int64_t>(0, 0)) == box<int64_t>({0, 0}, {0, 0}));
    CHECK(envelope(line_string<int64_t>({{0, 1}, {2, 3}})) == box<int64_t>({0, 1}, {2, 3}));
    CHECK(envelope(polygon<int64_t>({{{0, 1}, {2, 3}}})) == box<int64_t>({0, 1}, {2, 3}));

    CHECK(envelope(multi_point<int64_t>({{0, 0}})) == box<int64_t>({0, 0}, {0, 0}));
    CHECK(envelope(multi_line_string<int64_t>({{{0, 1}, {2, 3}}})) == box<int64_t>({0, 1}, {2, 3}));
    CHECK(envelope(multi_polygon<int64_t>({{{{0, 1}, {2, 3}}}})) == box<int64_t>({0, 1}, {2, 3}));

    CHECK(envelope(geometry<int>(point<int>(0, 0))) == box<int>({0, 0}, {0, 0}));
    CHECK(envelope(geometry_collection<int>({point<int>(0, 0)})) == box<int>({0, 0}, {0, 0}));
}
