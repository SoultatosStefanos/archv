#include "resources/config.hpp"
#include "utility/json.hpp"

#include <gtest/gtest.h>

using namespace resources;
using namespace testing;
using namespace utility;

namespace
{

auto sample_0_resources()
{
    return config_data { .meshes = { "a", "a" },
                         .materials = { "b" },
                         .fonts = { "c", "c", "c" } };
}

TEST(resources_deserialization, sample_resources_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_resources_0.json");
    const auto expected = sample_0_resources();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace