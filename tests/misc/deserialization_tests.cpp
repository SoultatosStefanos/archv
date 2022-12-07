#include "misc/deserialization.hpp"

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
    Json::Value val = make_jsoncpp_val();
};

TEST_F(given_a_json_val, values_can_be_accessed_from_valid_keys)
{
    EXPECT_NO_THROW(get(val, "bool"));
    EXPECT_NO_THROW(get(val, "int"));
    EXPECT_NO_THROW(get(val, "unsigned int"));
    EXPECT_NO_THROW(get(val, "int64_t"));
    EXPECT_NO_THROW(get(val, "uint64_t"));
    EXPECT_NO_THROW(get(val, "double"));
    EXPECT_NO_THROW(get(val, "string"));
}

TEST_F(given_a_json_val, values_can_be_accessed_from_their_types)
{
    EXPECT_EQ(as< bool >(get(val, "bool")), true);
    EXPECT_EQ(as< int >(get(val, "int")), 8);
    EXPECT_EQ(as< unsigned int >(get(val, "unsigned int")), 80u);
    EXPECT_EQ(as< int64_t >(get(val, "int64_t")), 9);
    EXPECT_EQ(as< uint64_t >(get(val, "uint64_t")), 23);
    EXPECT_EQ(as< double >(get(val, "double")), 7.0);
    EXPECT_EQ(as< std::string >(get(val, "string")), "foo");
}

TEST_F(
    given_a_json_val,
    a_key_not_found__error_is_thrown_when_accessing_with_invalid_key)
{
    EXPECT_THROW(get(val, "aaaaa"), key_not_found);
}

TEST_F(
    given_a_json_val,
    a_type_mismatch_error_is_thrown_when_accessing_with_invalid_type)
{
    EXPECT_THROW(as< bool >(get(val, "string")), type_mismatch);
}

} // namespace