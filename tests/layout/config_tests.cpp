
#include "../utility/config.hpp"
#include "layout/backend.hpp"
#include "layout/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace layout;
using namespace testing;
using namespace utility;

namespace
{

auto make_sample_layout_0()
{
    return config_data { .layouts = { "Gursoy Atun" },
                         .topologies = { "Cube" },
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

TEST(layout_serialization, sample_layout_0)
{
    auto root = json_val();
    const auto cfg = make_sample_layout_0();

    serialize(root, cfg);
    auto deserialized = deserialize(root);

    EXPECT_EQ(deserialized, cfg);
}

} // namespace