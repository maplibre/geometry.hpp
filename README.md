[![MapLibre Logo](https://maplibre.org/img/maplibre-logo-big.svg)](https://maplibre.org/)

# geometry.hpp

Provides header-only, generic C++ interfaces for geometry types, geometry collections, and features.

 - `maplibre::geometry::point`
 - `maplibre::geometry::multi_point`
 - `maplibre::geometry::line_string`
 - `maplibre::geometry::multi_line_string`
 - `maplibre::geometry::polygon`
 - `maplibre::geometry::multi_polygon`
 - `maplibre::geometry::geometry_collection`
 - `maplibre::feature::feature`

Dependency of [MapLibre Native](https://github.com/maplibre/maplibre-native).

### Design

These types are designed to be easy to parse and serialize to [GeoJSON](http://geojson.org/).

They should also be a robust and high performance container for data processing and conversion.


### Goals

 - Header-only
 - Fast compile
 - C++20
 - No external dependencies for usage of core types (point, line_string, etc)
 - No dependencies for usage of enclosing `geometry` type
 - Easily [adaptable to `boost::geometry`](http://www.boost.org/doc/libs/1_56_0/libs/geometry/doc/html/geometry/examples/example__adapting_a_legacy_geometry_object_model.html)


### Usage

Using a single type directly:

```cpp
#include <maplibre/geometry/point.hpp>
#include <iostream>

using maplibre::geometry::point;

int main() {
    point<double> pt(1.0,0.0);
    std::clog << "x: " << pt.x << " y: " << pt.y << "\n";
}
```

Creating a geometry collection:

```cpp
#include <maplibre/geometry/geometry.hpp>
#include <mapbox/variant.hpp>
#include <iostream>

using maplibre::geometry::geometry_collection;
using maplibre::geometry::geometry;
using maplibre::geometry::point;

using point_type = point<double>;

struct printer
{
    printer() {}

    void operator()(point_type const& pt) const
    {
        std::clog << "x: " << pt.x << " y: " << pt.y << "\n";
    }

    template <typename T>
    void operator()(T const& g) const
    {
        std::clog << "encountered non-point geometry\n";
    }
};

int main() {
    geometry_collection<double> gc;
    gc.emplace_back(point_type(1.0,0.0));
    geometry<double> const& geom = gc.at(0);
    printer visitor;
    mapbox::util::apply_visitor(visitor,geom);
}
```
