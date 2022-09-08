#include "layout/config.hpp"
#include "layout/layout_plugin.hpp"
#include "layout/topology_plugin.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace layout;
using namespace utility;
using namespace testing;

namespace
{

TEST(layout_deserialization, sample_layout_invalid)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-1.json");

    EXPECT_THROW(deserialize(root), deserialization_error);
}

TEST(layout_deserialization, sample_layout_unknown_layout)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-5.json");

    EXPECT_THROW(deserialize(root), unknown_layout);
}

TEST(layout_deserialization, sample_layout_unknown_topology)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-6.json");

    EXPECT_THROW(deserialize(root), unknown_topology);
}

auto make_sample_layout_0()
{
    return config_data { .layout = layout_plugin::gursoy_atun_id,
                         .topology = topology_plugin::cube_id,
                         .scale = 100 };
}

TEST(layout_deserialization, sample_layout_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_0.json");
    const auto expected = make_sample_layout_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(expected, actual);
}

} // namespace