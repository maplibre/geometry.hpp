#pragma once

#include <maplibre/geometry/empty.hpp>
#include <maplibre/feature.hpp>

#include <algorithm>
#include <iostream>
#include <string>

namespace maplibre {
namespace geometry {

inline std::ostream& operator<<(std::ostream& os, const empty&)
{
    return os << "[]";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const point<T>& point)
{
    return os << '[' << point.x << ',' << point.y << ']';
}

template <typename T, template <class, class...> class C, class... Args>
std::ostream& operator<<(std::ostream& os, const C<T, Args...>& cont)
{
    os << '[';
    for (auto it = cont.cbegin();;)
    {
        os << *it;
        if (++it == cont.cend())
        {
            break;
        }
        os << ',';
    }
    return os << ']';
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const line_string<T>& geom)
{
    return os << static_cast<typename line_string<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const linear_ring<T>& geom)
{
    return os << static_cast<typename linear_ring<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const polygon<T>& geom)
{
    return os << static_cast<typename polygon<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const multi_point<T>& geom)
{
    return os << static_cast<typename multi_point<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const multi_line_string<T>& geom)
{
    return os << static_cast<typename multi_line_string<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const multi_polygon<T>& geom)
{
    return os << static_cast<typename multi_polygon<T>::container_type>(geom);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const geometry<T>& geom)
{
    std::visit([&](const auto& g) { os << g; }, geom);
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const geometry_collection<T>& geom)
{
    return os << static_cast<typename geometry_collection<T>::container_type>(geom);
}

} // namespace geometry

namespace feature {

inline std::ostream& operator<<(std::ostream& os, const null_value_t&)
{
    return os << "null";
}

void to_stream(maplibre::feature::property_map const&, std::ostream& dest);

void to_stream(std::vector<maplibre::feature::value> const&, std::ostream& dest);

inline void quote_string(std::string const& in, std::ostream& dest)
{
    dest << '\"';
    for (char c : in)
    {
        if (c == '"' || c == '\\')
        {
            dest << '\\';
        }
        dest << c;
    }
    dest << '\"';
}

struct value_to_stream_visitor
{

    std::ostream& out;

    template <typename T>
    void operator()(T val)
    {
        out << val;
    }

    void operator()(std::string const& val)
    {
        quote_string(val, out);
    }

    void operator()(bool val)
    {
        out << (val ? "true" : "false");
    }

    void operator()(std::vector<maplibre::feature::value> const& vec)
    {
        out << '[';
        bool first = true;
        for (auto const& item : vec)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                out << ',';
            }
            std::visit(*this, item);
        }
        out << ']';
    }

    void operator()(std::shared_ptr<std::vector<maplibre::feature::value>> const& vec)
    {
        (*this)(*vec);
    }

    void operator()(std::unordered_map<std::string, maplibre::feature::value> const& map)
    {
        out << '{';
        std::vector<std::string> keys;
        for (auto const& p : map)
        {
            keys.push_back(p.first);
        }
        std::sort(keys.begin(), keys.end());
        bool first = true;
        for (auto const& k : keys)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                out << ',';
            }
            auto const val = map.find(k);
            quote_string(k, out);
            out << ':';
            std::visit(*this, val->second);
        }
        out << '}';
    }

    void operator()(std::shared_ptr<std::unordered_map<std::string, maplibre::feature::value>> const& map)
    {
        (*this)(*map);
    }
};

inline std::ostream& operator<<(std::ostream& os, std::unordered_map<std::string, maplibre::feature::value> const& map)
{
    value_to_stream_visitor vis{os};
    vis(map);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, std::vector<maplibre::feature::value> const& vec)
{
    value_to_stream_visitor vis{os};
    vis(vec);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, maplibre::feature::value const& val)
{
    std::visit(value_to_stream_visitor{os}, val);
    return os;
}

struct identifier_to_stream_visitor
{

    std::ostream& out;

    template <typename T>
    void operator()(T val)
    {
        out << val;
    }

    void operator()(std::string const& val)
    {
        quote_string(val, out);
    }
};

inline std::ostream& operator<<(std::ostream& os, maplibre::feature::identifier const& val)
{
    std::visit(identifier_to_stream_visitor{os}, val);
    return os;
}

} // namespace feature
} // namespace maplibre
