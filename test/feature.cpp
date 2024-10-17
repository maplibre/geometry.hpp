#include <catch2/catch_all.hpp>
#include <maplibre/feature.hpp>

using maplibre::geometry::point;

using maplibre::feature::feature;
using maplibre::feature::feature_collection;
using maplibre::feature::null_value;
using maplibre::feature::null_value_t;
using maplibre::feature::value;

namespace {

template <typename T, typename U>
void checkType(U arg)
try
{
    value v{arg};
    CHECK(v);
    CHECK(std::holds_alternative<T>(v));
    CHECK(std::get<T>(v) == arg);
}
catch (...)
{
    FAIL();
}

template <typename T, typename U>
void checkPtrType(U arg)
try
{
    value v{arg};
    CHECK(v);
    CHECK(std::holds_alternative<T>(v));
    CHECK(*(std::get<T>(v)) == *arg);
}
catch (...)
{
    FAIL();
}

template <typename T, typename U>
void checkPtrType2(U arg)
try
{
    value v{arg};
    CHECK(v);
    CHECK(std::holds_alternative<T>(v));
    CHECK(*(std::get<T>(v)) == arg);
}
catch (...)
{
    FAIL();
}

} // namespace

TEST_CASE("test value")
{
    CHECK(!value());
    checkType<int64_t>(32);
    checkType<uint64_t>(32u);
    checkType<bool>(false);
    checkType<std::string>("hello");

    value::array_type vec;
    vec.emplace_back(value(32));
    checkPtrType<value::array_ptr_type>(std::make_shared<value::array_type>(vec));
    checkPtrType2<value::array_ptr_type>(vec);
    value::object_type map;
    map.emplace("a", value(33));
    checkPtrType<value::object_ptr_type>(std::make_shared<value::object_type>(map));
    checkPtrType2<value::object_ptr_type>(map);

    value intV{32};
    CHECK_THROWS(std::get<uint64_t>(intV));

    auto* result = intV.getInt();
    CHECK(result);
    CHECK(*result == 32);
    *result = 100;
    CHECK(std::get<int64_t>(intV) == 100);

    CHECK_FALSE(intV.getUint());
    CHECK_FALSE(intV.getBool());
    CHECK_FALSE(intV.getDouble());
    CHECK_FALSE(intV.getArray());
    CHECK_FALSE(intV.getObject());
    CHECK_FALSE(intV.getString());
}

TEST_CASE("test feature")
{
    feature<int64_t> pf{point<int64_t>()};
    CHECK(std::holds_alternative<point<int64_t>>(pf.geometry));
    CHECK(pf.properties.empty());

    auto& p = pf.properties;

    p["bool"] = true;
    p["string"] = std::string("foo");
    p["double"] = 2.5;
    p["uint"] = uint64_t(10);
    p["int"] = int64_t(-10);
    p["null"] = null_value;

    REQUIRE(std::holds_alternative<bool>(p["bool"]));
    CHECK(p["bool"] == true);
    REQUIRE(std::holds_alternative<std::string>(p["string"]));
    CHECK(p["string"] == std::string("foo"));
    REQUIRE(std::holds_alternative<double>(p["double"]));
    CHECK(p["double"] == 2.5);
    REQUIRE(std::holds_alternative<uint64_t>(p["uint"]));
    CHECK(p["uint"] == uint64_t(10));
    REQUIRE(std::holds_alternative<int64_t>(p["int"]));
    CHECK(p["int"] == int64_t(-10));
    REQUIRE(std::holds_alternative<null_value_t>(p["null"]));
    CHECK(p["null"] == null_value);

    p["null"] = null_value_t{};
    REQUIRE(std::holds_alternative<null_value_t>(p["null"]));

    CHECK(p["null"] == null_value);

    CHECK(p == p);
    CHECK(!(p != p));

    CHECK(pf == pf);
    CHECK(!(pf != pf));

    CHECK(p.size() == 6);

    feature<int64_t> id1{point<int64_t>()};
    id1.id = {uint64_t(1)};

    feature<int64_t> id2{point<int64_t>()};
    id1.id = {uint64_t(2)};

    CHECK(id1 == id1);
    CHECK(id1 != id2);
}

TEST_CASE("test feature collection")
{
    feature_collection<int64_t> fc1;
    CHECK(fc1.empty());

    CHECK(fc1 == fc1);
    CHECK(!(fc1 != fc1));
}
