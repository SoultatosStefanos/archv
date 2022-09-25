#include "scaling/config.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace scaling;
using namespace utility;
using namespace testing;

namespace
{

auto make_sample_scaling_0()
{
    return config_data { { "something", make_z_factor(6, true) },
                         { "other", make_xz_factor(600, true) } };
}

TEST(scaling_deserialization, sample_scaling_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_scaling_0.json");
    const auto expected = make_sample_scaling_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace