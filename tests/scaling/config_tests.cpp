#include "../utility/config.hpp"
#include "scaling/config.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace scaling;
using namespace testing;
using namespace utility;

namespace
{

auto make_sample_scaling_0()
{
    return config_data { { "something", make_z_factor(6, true, 0, 0) },
                         { "other", make_xz_factor(600, true, 10, 110) } };
}

TEST(scaling_deserialization, sample_scaling_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_scaling_0.json");
    const auto expected = make_sample_scaling_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

TEST(scaling_serialization, sample_scaling_0)
{
    auto root = json_val();
    const auto cfg = make_sample_scaling_0();

    serialize(root, cfg);
    auto deserialized = deserialize(root);

    EXPECT_EQ(deserialized, cfg);
}

} // namespace