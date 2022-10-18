#include "../utility/config.hpp"
#include "degrees/config.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace degrees;
using namespace testing;
using namespace utility;

namespace
{

inline auto make_sample_degrees_0()
{
    return std::make_pair(
        effect_evaluation_data({ 2, 5, 10 }, { "aa", "bb", "cc" }, true),
        effect_evaluation_data({ 1, 2, 3 }, { "aaa", "bbb", "ccc" }, true));
}

TEST(degrees_deserialization, sample_degrees_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_degrees_0.json");
    const auto expected = make_sample_degrees_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace