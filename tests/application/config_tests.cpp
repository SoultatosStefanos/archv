#include "application/config.hpp"
#include "utility/all.hpp"

#include "json/all.hpp"
#include <gtest/gtest.h>

using namespace utility;
using namespace application;
using namespace testing;
using namespace json;

namespace
{

TEST(app_deserialization, sample_config_invalid)
{
    const auto root = read_json_root("../../data/tests/sample_config_-1.json");

    EXPECT_THROW(deserialize(root), deserialization_error);
}

auto make_sample_config_0()
{
    return config_data { .dependencies_path = "foo/bar/dependencies.json",
                         .layout_path = "foo/bar/layout.json" };
}

TEST(app_deserialization, sample_config_0)
{
    const auto root = read_json_root("../../data/tests/sample_config_0.json");
    const auto expected = make_sample_config_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(expected, actual);
}

} // namespace