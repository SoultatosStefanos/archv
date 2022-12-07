#include "misc/json_val.hpp"

#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>
#include <memory>

using namespace misc;
using namespace testing;

namespace
{

inline auto make_jsoncpp_val() -> Json::Value
{
    Json::Value val;
    val["bool"] = true;
    val["int"] = 8;
    val["unsigned int"] = 80u;
    val["int64_t"] = static_cast< int64_t >(9);
    val["uint64_t"] = static_cast< uint64_t >(23);
    val["double"] = 7.0;
    val["string"] = "foo";
    return val;
}

class given_a_json_val : public Test
{
protected:
    std::unique_ptr< json_val > val;
    Json::Value jsoncpp_val = make_jsoncpp_val();

    void SetUp() override { val = std::make_unique< json_val >(jsoncpp_val); }
};

TEST_F(given_a_json_val, values_can_be_accessed_from_valid_keys)
{
    EXPECT_NO_THROW((*val)["bool"]);
    EXPECT_NO_THROW((*val)["int"]);
    EXPECT_NO_THROW((*val)["unsigned int"]);
    EXPECT_NO_THROW((*val)["int64_t"]);
    EXPECT_NO_THROW((*val)["uint64_t"]);
    EXPECT_NO_THROW((*val)["double"]);
    EXPECT_NO_THROW((*val)["string"]);
}

TEST_F(given_a_json_val, values_can_be_queried_for_their_types)
{
    EXPECT_TRUE((*val)["bool"].is< bool >());
    EXPECT_TRUE((*val)["int"].is< int >());
    EXPECT_TRUE((*val)["unsigned int"].is< unsigned int >());
    EXPECT_TRUE((*val)["int64_t"].is< int64_t >());
    EXPECT_TRUE((*val)["uint64_t"].is< uint64_t >());
    EXPECT_TRUE((*val)["double"].is< double >());
    EXPECT_TRUE((*val)["string"].is< std::string >());

    EXPECT_TRUE((*val)["bool"].isBool());
    EXPECT_TRUE((*val)["int"].isInt());
    EXPECT_TRUE((*val)["unsigned int"].isUInt());
    EXPECT_TRUE((*val)["int64_t"].isInt64());
    EXPECT_TRUE((*val)["uint64_t"].isUInt64());
    EXPECT_TRUE((*val)["double"].isDouble());
    EXPECT_TRUE((*val)["string"].isString());

    EXPECT_FALSE((*val)["bool"].is< int >());
    EXPECT_FALSE((*val)["int"].is< std::string >());
    EXPECT_FALSE((*val)["unsigned int"].is< bool >());
    EXPECT_FALSE((*val)["int64_t"].is< bool >());
    EXPECT_FALSE((*val)["uint64_t"].is< bool >());
    EXPECT_FALSE((*val)["double"].is< bool >());
    EXPECT_FALSE((*val)["string"].is< bool >());

    EXPECT_FALSE((*val)["bool"].isInt64());
    EXPECT_FALSE((*val)["int"].isBool());
    EXPECT_FALSE((*val)["unsigned int"].isBool());
    EXPECT_FALSE((*val)["int64_t"].isBool());
    EXPECT_FALSE((*val)["uint64_t"].isString());
    EXPECT_FALSE((*val)["double"].isBool());
    EXPECT_FALSE((*val)["string"].isDouble());
}

TEST_F(given_a_json_val, values_can_be_accessed_from_their_types)
{
    EXPECT_EQ((*val)["bool"].as< bool >(), true);
    EXPECT_EQ((*val)["int"].as< int >(), 8);
    EXPECT_EQ((*val)["unsigned int"].as< unsigned int >(), 80u);
    EXPECT_EQ((*val)["int64_t"].as< int64_t >(), 9);
    EXPECT_EQ((*val)["uint64_t"].as< uint64_t >(), 23);
    EXPECT_EQ((*val)["double"].as< double >(), 7.0);
    EXPECT_EQ((*val)["string"].as< std::string >(), "foo");

    EXPECT_EQ((*val)["bool"].asBool(), true);
    EXPECT_EQ((*val)["int"].asInt(), 8);
    EXPECT_EQ((*val)["unsigned int"].asUInt(), 80u);
    EXPECT_EQ((*val)["int64_t"].asInt64(), 9);
    EXPECT_EQ((*val)["uint64_t"].asUInt64(), 23);
    EXPECT_EQ((*val)["double"].asDouble(), 7.0);
    EXPECT_EQ((*val)["string"].asString(), "foo");
}

TEST_F(given_a_json_val, values_can_be_iterated)
{
    const auto all = [](const auto&) { return true; };
    EXPECT_EQ(std::count_if(std::cbegin(*val), std::cend(*val), all), 7);
}

TEST_F(
    given_a_json_val,
    a_key_not_found__error_is_thrown_when_accessing_with_invalid_key)
{
    EXPECT_THROW((*val)["aaaaa"], key_not_found);
}

TEST_F(
    given_a_json_val,
    a_type_mismatch_error_is_thrown_when_accessing_with_invalid_type)
{
    EXPECT_THROW((*val)["string"].as< bool >(), type_mismatch);
}

} // namespace