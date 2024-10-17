#pragma once

#include <maplibre/geometry.hpp>

#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <unordered_map>

namespace maplibre {
namespace feature {

struct value;

struct null_value_t
{
};

constexpr bool operator==(const null_value_t&, const null_value_t&) { return true; }
constexpr bool operator!=(const null_value_t&, const null_value_t&) { return false; }
constexpr bool operator<(const null_value_t&, const null_value_t&) { return false; }

constexpr null_value_t null_value = null_value_t();

// helper class for std::visit
namespace {
template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
}; // namespace

struct value;

// Multiple numeric types (uint64_t, int64_t, double) are present in order to support
// the widest possible range of JSON numbers, which do not have a maximum range.
// Implementations that produce `value`s should use that order for type preference,
// using uint64_t for positive integers, int64_t for negative integers, and double
// for non-integers and integers outside the range of 64 bits.
using value_base = std::variant<null_value_t, bool, uint64_t, int64_t, double, std::string,
                                std::shared_ptr<std::vector<value>>,
                                std::shared_ptr<std::unordered_map<std::string, value>>>;

struct value : public value_base
{
    using array_type = std::vector<value>;
    using array_ptr_type = std::shared_ptr<std::vector<value>>;
    using const_array_ptr_type = std::shared_ptr<const std::vector<value>>;
    using object_type = std::unordered_map<std::string, value>;
    using object_ptr_type = std::shared_ptr<std::unordered_map<std::string, value>>;
    using const_object_ptr_type = std::shared_ptr<const std::unordered_map<std::string, value>>;

    value() : value_base(null_value) {}
    value(null_value_t) : value_base(null_value) {}
    value(bool v) : value_base(v) {}
    value(const char* c) : value_base(std::string(c)) {}
    value(std::string str) : value_base(std::move(str)) {}

    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0,
              typename std::enable_if_t<std::is_signed<T>::value, int> = 0>
    value(T t) : value_base(int64_t(t))
    {
    }

    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0,
              typename std::enable_if_t<!std::is_signed<T>::value, int> = 0>
    value(T t) : value_base(uint64_t(t))
    {
    }

    template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
    value(T t) : value_base(double(t))
    {
    }
    value(array_type array) : value_base(std::make_shared<array_type>(std::forward<array_type>(array))) {}
    value(array_ptr_type array) : value_base(array) {}
    value(object_type object) : value_base(std::make_shared<object_type>(std::forward<object_type>(object))) {}
    value(object_ptr_type object) : value_base(object) {}

    bool operator==(const value& rhs) const
    {
        if (index() != rhs.index()) return false;

        if (std::holds_alternative<value::array_ptr_type>(*this))
        {
            return *std::get<value::array_ptr_type>(*this) == *std::get<value::array_ptr_type>(rhs);
        }

        if (std::holds_alternative<value::object_ptr_type>(*this))
        {
            return *std::get<value::object_ptr_type>(*this) == *std::get<value::object_ptr_type>(rhs);
        }

        return *static_cast<const value_base*>(this) == rhs;
    }

    explicit operator bool() const { return !std::holds_alternative<null_value_t>(variant()); }

    array_ptr_type getArray() noexcept
    {
        return std::visit(overloaded{
                              [](array_ptr_type& val) -> array_ptr_type { return val; },
                              [](auto&) -> array_ptr_type { return nullptr; }},
                          variant());
    }

    const_array_ptr_type getArray() const noexcept
    {
        return const_cast<value*>(this)->getArray();
    }

    object_ptr_type getObject() noexcept
    {
        return std::visit(overloaded{
                              [](object_ptr_type& val) -> object_ptr_type { return val; },
                              [](auto&) -> object_ptr_type { return nullptr; }},
                          variant());
    }

    const_object_ptr_type getObject() const noexcept
    {
        return const_cast<value*>(this)->getObject();
    }

    value_base& variant()
    {
        return *static_cast<value_base*>(this);
    }

    value_base const& variant() const
    {
        return *static_cast<const value_base*>(this);
    }

    int64_t* getInt() noexcept
    {
        return std::visit(overloaded{
                              [](int64_t& val) -> int64_t* { return &val; },
                              [](auto&) -> int64_t* { return nullptr; }},
                          variant());
    }

    const int64_t* getInt() const noexcept
    {
        return const_cast<value*>(this)->getInt();
    }

    uint64_t* getUint() noexcept
    {
        return std::visit(overloaded{
                              [](uint64_t& val) -> uint64_t* { return &val; },
                              [](auto&) -> uint64_t* { return nullptr; }},
                          variant());
    }

    const uint64_t* getUint() const noexcept
    {
        return const_cast<value*>(this)->getUint();
    }

    bool* getBool() noexcept
    {
        return std::visit(overloaded{
                              [](bool& val) -> bool* { return &val; },
                              [](auto&) -> bool* { return nullptr; }},
                          variant());
    }

    const bool* getBool() const noexcept
    {
        return const_cast<value*>(this)->getBool();
    }
    double* getDouble() noexcept
    {
        return std::visit(overloaded{
                              [](double& val) -> double* { return &val; },
                              [](auto&) -> double* { return nullptr; }},
                          variant());
    }

    const double* getDouble() const noexcept
    {
        return const_cast<value*>(this)->getDouble();
    }
    std::string* getString() noexcept
    {
        return std::visit(overloaded{
                              [](std::string& val) -> std::string* { return &val; },
                              [](auto&) -> std::string* { return nullptr; }},
                          variant());
    }

    const std::string* getString() const noexcept
    {
        return const_cast<value*>(this)->getString();
    }
};

using property_map = value::object_type;

// The same considerations and requirement for numeric types apply as for `value_base`.
using identifier = std::variant<null_value_t, uint64_t, int64_t, double, std::string>;

template <class T>
struct feature
{
    using coordinate_type = T;
    using geometry_type = maplibre::geometry::geometry<T>; // Fully qualified to avoid GCC -fpermissive error.

    geometry_type geometry;
    property_map properties;
    identifier id;

    feature()
        : geometry(),
          properties(),
          id() {}
    feature(geometry_type const& geom_)
        : geometry(geom_),
          properties(),
          id() {}
    feature(geometry_type&& geom_)
        : geometry(std::move(geom_)),
          properties(),
          id() {}
    feature(geometry_type const& geom_, property_map const& prop_)
        : geometry(geom_), properties(prop_), id() {}
    feature(geometry_type&& geom_, property_map&& prop_)
        : geometry(std::move(geom_)),
          properties(std::move(prop_)),
          id() {}
    feature(geometry_type const& geom_, property_map const& prop_, identifier const& id_)
        : geometry(geom_),
          properties(prop_),
          id(id_) {}
    feature(geometry_type&& geom_, property_map&& prop_, identifier&& id_)
        : geometry(std::move(geom_)),
          properties(std::move(prop_)),
          id(std::move(id_)) {}
};

template <class T>
constexpr bool operator==(feature<T> const& lhs, feature<T> const& rhs)
{
    return lhs.id == rhs.id && lhs.geometry == rhs.geometry && lhs.properties == rhs.properties;
}

template <class T>
constexpr bool operator!=(feature<T> const& lhs, feature<T> const& rhs)
{
    return !(lhs == rhs);
}

template <class T, template <typename...> class Cont = std::vector>
struct feature_collection : Cont<feature<T>>
{
    using coordinate_type = T;
    using feature_type = feature<T>;
    using container_type = Cont<feature_type>;
    using size_type = typename container_type::size_type;

    template <class... Args>
    feature_collection(Args&&... args) : container_type(std::forward<Args>(args)...)
    {
    }
    feature_collection(std::initializer_list<feature_type> args)
        : container_type(std::move(args)) {}
};

} // namespace feature
} // namespace maplibre
