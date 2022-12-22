#include "config.hpp"

#include "misc/deserialization.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>
#include <string>

namespace rendering
{

using namespace misc;

namespace
{
    template < typename T >
    auto deserialize_triad(const json_val& val)
    {
        auto extract_val = [iter = std::begin(val)]() mutable -> T
        {
            auto val = as< T >(*iter);
            std::advance(iter, 1);
            return val;
        };

        auto&& f = extract_val();
        auto&& s = extract_val();
        auto&& t = extract_val();

        return std::make_tuple(std::move(f), std::move(s), std::move(t));
    }

    inline auto deserialize_rgb(const json_val& val) -> Ogre::ColourValue
    {
        using color_value = double;

        static_assert(std::is_constructible_v<
                      Ogre::ColourValue,
                      color_value,
                      color_value,
                      color_value >);

        const auto [r, g, b] = deserialize_triad< color_value >(val);

        BOOST_LOG_TRIVIAL(debug)
            << "extracted color value: (" << r << ", " << g << ", " << b << ')';

        return Ogre::ColourValue(r, g, b);
    }

    auto deserialize_background(const json_val& val) -> background_config
    {
        using real = double;
        using string = std::string;

        static_assert(std::is_convertible_v< real, Ogre::Real >);
        static_assert(std::is_convertible_v< string, Ogre::String >);

        auto&& skybox_material = as< string >(get(val, "skybox-material"));
        auto skybox_dist = as< real >(get(val, "skybox-distance"));
        auto&& ambient_color = deserialize_rgb(get(val, "ambient-color"));
        auto&& diffuse_color = deserialize_rgb(get(val, "diffuse-color"));
        auto&& specular_color = deserialize_rgb(get(val, "specular-color"));
        auto cam_near_clip_dist
            = as< real >(get(val, "cam-near-clip-distance"));
        auto cam_far_clip_dist = as< real >(get(val, "cam-far-clip-distance"));

        BOOST_LOG_TRIVIAL(debug) << "deserialized rendering background";

        return { std::move(skybox_material),
                 static_cast< Ogre::Real >(skybox_dist),
                 ambient_color,
                 diffuse_color,
                 specular_color,
                 static_cast< Ogre::Real >(cam_near_clip_dist),
                 static_cast< Ogre::Real >(cam_far_clip_dist) };
    }

    inline auto deserialize_vector3(const json_val& val) -> Ogre::Vector3
    {
        using real = double;

        const auto [xs, ys, zs] = deserialize_triad< real >(val);

        BOOST_LOG_TRIVIAL(debug) << "deserialized vector3: (" << xs << ", "
                                 << ys << ", " << zs << ')';

        return Ogre::Vector3(xs, ys, zs);
    }

    auto deserialize_graph(const json_val& val) -> graph_config
    {
        using string = std::string;
        using real = double;

        static_assert(std::is_convertible_v< string, Ogre::String >);

        auto&& vertex_mesh = as< string >(get(val, "vertex-mesh"));
        auto&& vertex_material = as< string >(get(val, "vertex-material"));
        auto&& vertex_scale = deserialize_vector3(get(val, "vertex-scale"));

        const auto& vboard_val = get(val, "vertex-id");
        auto&& vboard_font_name = as< string >(get(vboard_val, "font-name"));
        auto vboard_char_height = as< real >(get(vboard_val, "char-height"));
        auto&& vboard_color = deserialize_rgb(get(vboard_val, "color"));
        auto vboard_space_width = as< real >(get(vboard_val, "space-width"));

        auto&& edge_material = as< string >(get(val, "edge-material"));
        auto&& edge_tip_mesh = as< string >(get(val, "edge-tip-mesh"));
        auto&& edge_tip_mat = as< string >(get(val, "edge-tip-material"));
        auto&& edge_tip_scale = deserialize_vector3(get(val, "edge-tip-scale"));

        const auto& edgetype_val = get(val, "edge-type");
        auto&& edgetype_font_name
            = as< string >(get(edgetype_val, "font-name"));
        auto edgetype_char_height
            = as< real >(get(edgetype_val, "char-height"));
        auto&& edgetype_color = deserialize_rgb(get(edgetype_val, "color"));
        auto edgetype_space_width
            = as< real >(get(edgetype_val, "space-width"));

        BOOST_LOG_TRIVIAL(debug) << "deserialized rendering graph";

        return { std::move(vertex_mesh),
                 std::move(vertex_material),
                 vertex_scale,
                 std::move(vboard_font_name),
                 static_cast< float >(vboard_char_height),
                 vboard_color,
                 static_cast< float >(vboard_space_width),
                 std::move(edge_material),
                 std::move(edge_tip_mesh),
                 std::move(edge_tip_mat),
                 edge_tip_scale,
                 std::move(edgetype_font_name),
                 static_cast< float >(edgetype_char_height),
                 edgetype_color,
                 static_cast< float >(edgetype_space_width) };
    }

    template < typename T >
    inline auto deserialize_degree_ranks(const json_val& val)
    {
        auto&& light = as< T >(get(val, "light"));
        auto&& medium = as< T >(get(val, "medium"));
        auto&& heavy = as< T >(get(val, "heavy"));

        return std::make_tuple(
            std::move(light), std::move(medium), std::move(heavy));
    }

    inline auto deserialize_degrees_section(const json_val& val)
    {
        using threshold_type = degrees_ranked_evaluation_data::threshold_type;
        using system_type
            = degrees_ranked_evaluation_data::particle_system_type;
        using applied_type = degrees_ranked_evaluation_data::applied_type;

        auto&& [light_threshold, medium_threshold, heavy_threshold]
            = deserialize_degree_ranks< threshold_type >(
                get(val, "thresholds"));

        auto&& [light_particles, medium_particles, heavy_particles]
            = deserialize_degree_ranks< system_type >(
                get(val, "particle-systems"));

        const auto applied = as< applied_type >(get(val, "applied"));

        BOOST_LOG_TRIVIAL(debug) << "deserialized degree effects";

        return degrees_ranked_evaluation_data(
            make_ranked(light_threshold, medium_threshold, heavy_threshold),
            make_ranked(
                std::move(light_particles),
                std::move(medium_particles),
                std::move(heavy_particles)),
            applied);
    }

    inline auto deserialize_degrees(const json_val& root)
        -> degrees_ranked_config
    {
        return degrees_ranked_config(
            deserialize_degrees_section(get(root, "in-degree")),
            deserialize_degrees_section(get(root, "out-degree")));
    }

    inline auto deserialize_minimap(const json_val& root) -> minimap_config
    {
        using coord_type = minimap_config::coord_type;
        using dist_type = minimap_config::dist_type;

        const auto left = as< double >(get(root, "left"));
        const auto top = as< double >(get(root, "top"));
        const auto right = as< double >(get(root, "right"));
        const auto bottom = as< double >(get(root, "bottom"));
        const auto bkg_col = deserialize_rgb(get(root, "background-color"));
        const auto zoom_out = as< double >(get(root, "zoom-out"));
        const auto render_shadows = as< bool >(get(root, "render-shadows"));
        const auto render_sky = as< bool >(get(root, "render-sky"));
        const auto render_vertices = as< bool >(get(root, "render-vertices"));
        const auto render_vertex_ids
            = as< bool >(get(root, "render-vertex-ids"));
        const auto render_edges = as< bool >(get(root, "render-edges"));
        const auto render_edge_types
            = as< bool >(get(root, "render-edge-types"));
        const auto render_edge_tips = as< bool >(get(root, "render-edge-tips"));
        const auto render_particles = as< bool >(get(root, "render-particles"));

        return minimap_config(
            left,
            top,
            right,
            bottom,
            bkg_col,
            zoom_out,
            render_shadows,
            render_sky,
            render_vertices,
            render_vertex_ids,
            render_edges,
            render_edge_types,
            render_edge_tips,
            render_particles);
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    auto&& bkg = deserialize_background(get(root, "background"));
    auto&& g = deserialize_graph(get(root, "graph"));
    auto&& degrees = deserialize_degrees(get(root, "degrees"));
    auto&& minimap = deserialize_minimap(get(root, "minimap"));

    return config_data { .background = std::move(bkg),
                         .graph = std::move(g),
                         .degrees = std::move(degrees),
                         .minimap = minimap };
}

} // namespace rendering