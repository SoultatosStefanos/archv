#include "../utility/config.hpp"
#include "rendering/config.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

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
                              .ambient_color = ColourValue(0, 0, 0),
                              .diffuse_color = ColourValue(1, 1, 1),
                              .specular_color = ColourValue(2.5, 2.5, 2.5),
                              .cam_near_clip_distance = 3,
                              .cam_far_clip_distance = 3 };

    auto&& graph = graph_config { .vertex_mesh = "mesh.mesh",
                                  .vertex_material = "mat.mat",
                                  .vertex_scale = Vector3(0.1, 0.2, 0.3),
                                  .vertex_id_font_name = "a",
                                  .vertex_id_char_height = 1.0,
                                  .vertex_id_color = ColourValue(1.0, 1.0, 1.0),
                                  .vertex_id_space_width = 1.0,
                                  .edge_material = "mat",
                                  .edge_tip_mesh = "a",
                                  .edge_tip_material = "bob",
                                  .edge_tip_scale = Vector3(1, 1, 1),
                                  .edge_type_font_name = "e",
                                  .edge_type_char_height = 1.0,
                                  .edge_type_color = ColourValue(1.0, 1.0, 1.0),
                                  .edge_type_space_width = 1.0 };

    auto&& degrees = degrees_ranked_config {
        .in_data = degrees_ranked_evaluation_data(
            { 2, 5, 10 }, { "aa", "bb", "cc" }, true),
        .out_data = degrees_ranked_evaluation_data(
            { 1, 2, 3 }, { "aaa", "bbb", "ccc" }, true)
    };

    auto&& minimap
        = minimap_config { .left = 0,
                           .top = 0.2,
                           .right = 0.22,
                           .bottom = -1,
                           .background_col = ColourValue(1.0, 1.0, 1.0),
                           .zoom_out = 700,
                           .render_shadows = true,
                           .render_sky = true,
                           .render_vertices = true,
                           .render_vertex_ids = true,
                           .render_edges = true,
                           .render_edge_types = true,
                           .render_edge_tips = true,
                           .render_particles = true };

    return config_data { .background = std::move(background),
                         .graph = std::move(graph),
                         .degrees = std::move(degrees),
                         .minimap = std::move(minimap) };
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