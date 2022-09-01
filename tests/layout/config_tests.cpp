#include "layout/config.hpp"
#include "utility/all.hpp"

#include "json/all.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace layout;
using namespace utility;
using namespace json;
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
        = read_json_root("../../data/testfiles/sample_layout_-2.json");

    EXPECT_THROW(deserialize(root), unknown_layout);
}

TEST(layout_deserialization, sample_layout_unknown_topology)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-3.json");

    EXPECT_THROW(deserialize(root), unknown_topology);
}

TEST(layout_deserialization, sample_layout_invalid_negative_scale)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-4.json");

    EXPECT_THROW(deserialize(root), invalid_negative_scale);
}

TEST(layout_deserialization, sample_layout_unknown_default_layout)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-5.json");

    EXPECT_THROW(deserialize(root), unknown_default);
}

TEST(layout_deserialization, sample_layout_unknown_default_topology)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-6.json");

    EXPECT_THROW(deserialize(root), unknown_default);
}

TEST(layout_deserialization, sample_layout_unknown_default_scale)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_layout_-7.json");

    EXPECT_THROW(deserialize(root), unknown_default);
}

auto make_sample_layout_0()
{
    return config_data { .layouts = { "Gursoy Atun" },
                         .topologies = { "Cube" },
                         .scales = { 100 },
                         .default_layout = "Gursoy Atun",
                         .default_topology = "Cube",
                         .default_scale = 100 };
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