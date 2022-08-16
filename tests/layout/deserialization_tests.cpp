#include "layout/deserialization.hpp"
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
    const auto root = read_json_root("../../data/tests/sample_layout_-1.json");

    EXPECT_THROW(deserialize_layout(root), deserialization_error);
}

} // namespace