#include "config/deserialization.hpp"
#include "utility/all.hpp"

#include "json/all.hpp"
#include <gtest/gtest.h>

using namespace utility;
using namespace config;
using namespace testing;
using namespace json;

namespace
{

TEST(config_deserialization, sample_config_invalid)
{
    const auto root = read_json_root("../../data/tests/sample_config_-1.json");

    EXPECT_THROW(deserialize_config(root), deserialization_error);
}

} // namespace