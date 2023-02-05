#include "../utility/config.hpp"
#include "color_coding/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace color_coding;
using namespace utility;
using namespace testing;

namespace
{

auto make_sample_color_coding_0()
{
    return config_data { { "strong", color_code({ { 0, 0, 0, 1 } }, true) },
                         { "weak", color_code({ { 1, 1, 1, 1 } }, true) } };
}

TEST(color_coding_deserialization, sample_color_coding_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_color_coding_0.json");
    const auto expected = make_sample_color_coding_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

TEST(color_coding_serialization, sample_color_coding_0)
{
    auto root = json_val();
    const auto cfg = make_sample_color_coding_0();

    serialize(root, cfg);
    auto deserialized = deserialize(root);

    EXPECT_EQ(deserialized, cfg);
}

} // namespace