
#include "../utility/config.hpp"
#include "layout/backend.hpp"
#include "layout/config.hpp"
#include "layout/layout_plugin.hpp"
#include "layout/topology_plugin.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace layout;
using namespace testing;
using namespace utility;

namespace
{

TEST(layout_deserialization, sample_layout_invalid)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-1.json");

    EXPECT_THROW(deserialize(root), deserialization_error);
}

TEST(layout_deserialization, sample_layout_unknown_plugin_layout)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-2.json");

    EXPECT_THROW(deserialize(root), unknown_plugin);
}

TEST(layout_deserialization, sample_layout_unknown_plugin_topology)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-3.json");

    EXPECT_THROW(deserialize(root), unknown_plugin);
}

TEST(layout_deserialization, sample_layout_unlisted_default_scale)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-4.json");

    EXPECT_THROW(deserialize(root), unlisted_default);
}

TEST(layout_deserialization, sample_layout_unlisted_default_layout)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-5.json");

    EXPECT_THROW(deserialize(root), unlisted_default);
}

TEST(layout_deserialization, sample_layout_unlisted_default_topology)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-6.json");

    EXPECT_THROW(deserialize(root), unlisted_default);
}

auto make_sample_layout_0()
{
    return config_data { .layouts = { "Gursoy Atun" },
                         .topologies = { "Cube" },
                         .scales = { 0, 2000 },
                         .layout = "Gursoy Atun",
                         .topology = "Cube",
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