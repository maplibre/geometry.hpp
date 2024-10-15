#include <catch2/catch_all.hpp>
#include <maplibre/geometry/empty.hpp>

using maplibre::geometry::empty;

TEST_CASE("test empty")
{
    empty n1;
    empty n2;
    CHECK(n1 == n2);
    CHECK(!(n1 != n2));
    CHECK(!(n1 < n2));
    CHECK(!(n1 > n2));
    CHECK(n1 <= n2);
    CHECK(n1 >= n2);
}
