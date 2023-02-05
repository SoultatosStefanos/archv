#include "../utility/config.hpp"
#include "degrees/backend_config.hpp"
#include "degrees/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace degrees;
using namespace testing;
using namespace utility;

namespace
{

inline auto make_sample_degrees_0()
{
    return config_data {
        .in_data = evaluation_data({ 2, 5, 10 }, { "aa", "bb", "cc" }, true),
        .out_data = evaluation_data({ 1, 2, 3 }, { "aaa", "bbb", "ccc" }, true)
    };
}

TEST(degrees_deserialization, sample_degrees_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_degrees_0.json");
    const auto expected = make_sample_degrees_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

TEST(degrees_serialization, sample_degrees_0)
{
    auto root = json_val();
    const auto cfg = make_sample_degrees_0();

    serialize(root, cfg);
    auto deserialized = deserialize(root);

    EXPECT_EQ(deserialized, cfg);
}

} // namespace