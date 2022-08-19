#include "dependencies/config.hpp"
#include "utility/all.hpp"

#include "json/all.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace dependencies;
using namespace utility;
using namespace json;
using namespace testing;

namespace
{

TEST(dependencies_deserialization, sample_dependencies_invalid)
{
    const auto root
        = read_json_root("../../data/tests/sample_dependencies_-1.json");

    EXPECT_THROW(deserialize(root), deserialization_error);
}

auto make_sample_dependencies_0()
{
    return config_data { { "strong", 10 }, { "weak", 2 } };
}

TEST(dependencies_deserialization, sample_dependencies_0)
{
    const auto root
        = read_json_root("../../data/tests/sample_dependencies_0.json");
    const auto expected = make_sample_dependencies_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace