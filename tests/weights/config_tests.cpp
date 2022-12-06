#include "../utility/config.hpp"
#include "weights/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace weights;
using namespace utility;
using namespace testing;

namespace
{

auto make_sample_weights_0()
{
    return config_data { { "strong", 10 }, { "weak", 2 } };
}

TEST(weights_deserialization, sample_weights_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_weights_0.json");
    const auto expected = make_sample_weights_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace