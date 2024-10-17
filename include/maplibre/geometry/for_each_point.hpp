#pragma once

#include <maplibre/geometry/geometry.hpp>

namespace maplibre {
namespace geometry {

template <typename F>
void for_each_point(maplibre::geometry::empty const&, F&&)
{
}

template <typename Point, typename F>
auto for_each_point(Point&& point, F&& f)
    -> decltype(point.x, point.y, void())
{
    f(std::forward<Point>(point));
}

template <typename Container, typename F>
auto for_each_point(Container&& container, F&& f)
    -> decltype(container.begin(), container.end(), void());

template <typename... Types, typename F>
void for_each_point(std::variant<Types...> const& geom, F&& f)
{
    std::visit([&](auto const& g) {
        for_each_point(g, f);
    },
               geom);
}

template <typename... Types, typename F>
void for_each_point(std::variant<Types...>& geom, F&& f)
{
    std::visit([&](auto& g) {
        for_each_point(g, f);
    },
               geom);
}

template <typename Container, typename F>
auto for_each_point(Container&& container, F&& f)
    -> decltype(container.begin(), container.end(), void())
{
    for (auto& e : container)
    {
        for_each_point(e, f);
    }
}

} // namespace geometry
} // namespace maplibre
