# geometry.hpp

[![badge](https://raw.githubusercontent.com/mapbox/cpp/master/assets/hpp-skel-badge_blue.svg)](https://github.com/mapbox/hpp-skel)
[![Build Status](https://travis-ci.com/maplibre/geometry.hpp.svg?branch=master)](https://travis-ci.com/maplibre/geometry.hpp)

Provides header-only, generic C++ interfaces for geometry types, geometry collections, and features.

 - `maplibre::geometry::point`
 - `maplibre::geometry::multi_point`
 - `maplibre::geometry::line_string`
 - `maplibre::geometry::multi_line_string`
 - `maplibre::geometry::polygon`
 - `maplibre::geometry::multi_polygon`
 - `maplibre::geometry::geometry_collection`
 - `maplibre::feature::feature` (experimental)

### Design

These types are designed to be easy to parse and serialize to [GeoJSON](http://geojson.org/).

They should also be a robust and high performance container for data processing and conversion.


### Goals

 - Header-only
 - Fast compile
 - c++11/c++14 compatibility
 - No external dependencies for usage of core types (point, line_string, etc)
 - Minimal dependencies for usage of enclosing `geometry` type (`maplibre::variant`)
 - Easily [adaptable to `boost::geometry`](http://www.boost.org/doc/libs/1_56_0/libs/geometry/doc/html/geometry/examples/example__adapting_a_legacy_geometry_object_model.html)


### Usage

Using a single type directly (requires no external dependencies):

```cpp
#include <maplibre/geometry/point.hpp>
#include <iostream>

using maplibre::geometry::point;

int main() {
    point<double> pt(1.0,0.0);
    std::clog << "x: " << pt.x << " y: " << pt.y << "\n";
}
```

Creating a geometry collection (depends on https://github.com/mapbox/variant):

```cpp
#include <maplibre/geometry/geometry.hpp>
#include <maplibre/variant.hpp>
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
    maplibre::util::apply_visitor(visitor,geom);
}
```
