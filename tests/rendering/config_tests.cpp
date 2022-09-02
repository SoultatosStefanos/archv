#include "rendering/config.hpp"
#include "utility/json.hpp"

#include "json/deserialization.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace rendering;
using namespace testing;
using namespace utility;
using namespace json;

namespace
{

TEST(rendering_deserialization, sample_rendering_invalid)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_rendering_-1.json");

    ASSERT_THROW(deserialize(root), deserialization_error);
}

auto make_sample_rendering_0()
{
    using namespace Ogre;

    auto background
        = background_config { .skybox_material = "foo",
                              .skybox_distance = 10,
                              .ambient_light = ColourValue(0, 0, 0),
                              .diffuse_light = ColourValue(1, 1, 1),
                              .specular_light = ColourValue(2.5, 2.5, 2.5),
                              .near_clip_distance = 3,
                              .far_clip_distance = 3 };

    return config_data { .background = std::move(background) };
}

TEST(rendering_deserialization, sample_rendering_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_rendering_0.json");
    const auto expected = make_sample_rendering_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace