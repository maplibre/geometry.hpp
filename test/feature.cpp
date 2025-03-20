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
    CHECK(v.template is<T>());
    CHECK(v.template get<T>() == arg);
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
    CHECK(v.template is<T>());
    CHECK(*(v.template get<T>()) == *arg);
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
    CHECK(v.template is<T>());
    CHECK(*(v.template get<T>()) == arg);
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
    CHECK_THROWS(intV.get<uint64_t>());

    auto* result = intV.getInt();
    CHECK(result);
    CHECK(*result == 32);
    *result = 100;
    CHECK(intV.get<int64_t>() == 100);

    CHECK_FALSE(intV.getUint());
    CHECK_FALSE(intV.getBool());
    CHECK_FALSE(intV.getDouble());
    CHECK_FALSE(intV.getArray());
    CHECK_FALSE(intV.getObject());
    CHECK_FALSE(intV.getString());
}

TEST_CASE("test value operator==")
{
    value true1 = true;
    value true2 = true;
    value false1 = false;
    CHECK(true1 == true2);
    CHECK(true1 != false1);

    value::array_type vec = {1, 2, 3};
    value arr1 = std::make_shared<value::array_type>(vec);
    value arr2 = std::make_shared<value::array_type>(vec);
    CHECK(arr1 == arr2);

    value::object_type obj = {{"hello", 1}, {"world", true}};
    value obj1 = std::make_shared<value::object_type>(obj);
    value obj2 = std::make_shared<value::object_type>(obj);
    CHECK(obj1 == obj2);
}

TEST_CASE("test feature")
{
    feature<int64_t> pf{point<int64_t>()};
    CHECK(pf.geometry.is<point<int64_t>>());
    CHECK(pf.properties.empty());

    auto& p = pf.properties;

    p["bool"] = true;
    p["string"] = std::string("foo");
    p["double"] = 2.5;
    p["uint"] = uint64_t(10);
    p["int"] = int64_t(-10);
    p["null"] = null_value;

    REQUIRE(p["bool"].is<bool>());
    CHECK(p["bool"] == true);
    REQUIRE(p["string"].is<std::string>());
    CHECK(p["string"] == std::string("foo"));
    REQUIRE(p["double"].is<double>());
    CHECK(p["double"] == 2.5);
    REQUIRE(p["uint"].is<uint64_t>());
    CHECK(p["uint"] == uint64_t(10));
    REQUIRE(p["int"].is<int64_t>());
    CHECK(p["int"] == int64_t(-10));
    REQUIRE(p["null"].is<null_value_t>());
    CHECK(p["null"] == null_value);

    p["null"] = null_value_t{};
    REQUIRE(p["null"].is<null_value_t>());
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
