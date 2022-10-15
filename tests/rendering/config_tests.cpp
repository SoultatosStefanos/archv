#include "../utility/config.hpp"
#include "rendering/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace rendering;
using namespace testing;
using namespace utility;

namespace
{

auto make_sample_rendering_0()
{
    using namespace Ogre;

    auto&& background
        = background_config { .skybox_material = "foo",
                              .skybox_distance = 10,
                              .ambient_light = ColourValue(0, 0, 0),
                              .diffuse_light = ColourValue(1, 1, 1),
                              .specular_light = ColourValue(2.5, 2.5, 2.5),
                              .near_clip_distance = 3,
                              .far_clip_distance = 3 };

    auto&& graph
        = graph_config { .vertex_mesh = "mesh.mesh",
                         .vertex_scale = Vector3(0.1, 0.2, 0.3),
                         .vertex_id_font_name = "a",
                         .vertex_id_char_height = 1.0,
                         .vertex_id_color = ColourValue(1.0, 1.0, 1.0),
                         .vertex_id_space_width = 1.0,
                         .edge_material = "mat",
                         .edge_tip_mesh = "a",
                         .edge_tip_scale = Vector3(1, 1, 1),
                         .edge_type_font_name = "e",
                         .edge_type_char_height = 1.0,
                         .edge_type_color = ColourValue(1.0, 1.0, 1.0),
                         .edge_type_space_width = 1.0,
                         .in_degree_effects = vertex_degree_effects(
                             { { 2, 5, 10 } }, { { "aa", "bb", "cc" } }),
                         .out_degree_effects = vertex_degree_effects(
                             { { 1, 2, 3 } }, { { "aaa", "bbb", "ccc" } }) };

    return config_data { .background = std::move(background),
                         .graph = std::move(graph) };
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