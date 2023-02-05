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

    inline auto deserialize_vector3(const json_val& val) -> Ogre::Vector3
    {
        using real = double;

        const auto [xs, ys, zs] = deserialize_triad< real >(val);

        BOOST_LOG_TRIVIAL(debug) << "deserialized vector3: (" << xs << ", "
                                 << ys << ", " << zs << ')';

        return Ogre::Vector3(xs, ys, zs);
    }

} // namespace

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
    auto cam_near_clip_dist = as< real >(get(val, "cam-near-clip-distance"));
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
    auto&& edgetype_font_name = as< string >(get(edgetype_val, "font-name"));
    auto edgetype_char_height = as< real >(get(edgetype_val, "char-height"));
    auto&& edgetype_color = deserialize_rgb(get(edgetype_val, "color"));
    auto edgetype_space_width = as< real >(get(edgetype_val, "space-width"));

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

auto deserialize_minimap(const json_val& root) -> minimap_config
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
    const auto render_vertex_ids = as< bool >(get(root, "render-vertex-ids"));
    const auto render_edges = as< bool >(get(root, "render-edges"));
    const auto render_edge_types = as< bool >(get(root, "render-edge-types"));
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

auto deserialize(const json_val& root) -> config_data
{
    auto&& bkg = deserialize_background(get(root, "background"));
    auto&& g = deserialize_graph(get(root, "graph"));
    auto&& minimap = deserialize_minimap(get(root, "minimap"));

    return config_data { .background = std::move(bkg),
                         .graph = std::move(g),
                         .minimap = minimap };
}

namespace
{

    template < typename Tuple >
    inline auto serialize_triad(json_val& root, const Tuple& tuple)
    {
        static_assert(std::tuple_size_v< Tuple > == 3);

        root[0] = std::get< 0 >(tuple);
        root[1] = std::get< 1 >(tuple);
        root[2] = std::get< 2 >(tuple);
    }

    inline auto serialize_rgb(json_val& root, const Ogre::ColourValue& col)
    {
        serialize_triad(root, std::make_tuple(col.r, col.g, col.b));
    }

    inline auto serialize_vec3(json_val& root, const Ogre::Vector3& vec)
    {
        serialize_triad(root, std::make_tuple(vec.x, vec.y, vec.z));
    }

} // namespace

auto serialize_background(json_val& root, const background_config& cfg) -> void
{
    root["skybox-material"] = cfg.skybox_material;
    root["skybox-distance"] = cfg.skybox_distance;
    serialize_rgb(root["ambient-color"], cfg.ambient_color);
    serialize_rgb(root["diffuse-color"], cfg.diffuse_color);
    serialize_rgb(root["specular-color"], cfg.specular_color);
    root["cam-near-clip-distance"] = cfg.cam_near_clip_distance;
    root["cam-far-clip-distance"] = cfg.cam_far_clip_distance;

    BOOST_LOG_TRIVIAL(debug) << "serialized rendering background";
}

auto serialize_graph(json_val& root, const graph_config& cfg) -> void
{
    root["vertex-mesh"] = cfg.vertex_mesh;
    root["vertex-material"] = cfg.vertex_material;
    serialize_vec3(root["vertex-scale"], cfg.vertex_scale);

    auto& vertex_id_val = root["vertex-id"];
    vertex_id_val["font-name"] = cfg.vertex_id_font_name;
    vertex_id_val["char-height"] = cfg.vertex_id_char_height;
    serialize_rgb(vertex_id_val["color"], cfg.vertex_id_color);
    vertex_id_val["space-width"] = cfg.vertex_id_space_width;

    root["edge-material"] = cfg.edge_material;
    root["edge-tip-mesh"] = cfg.edge_tip_mesh;
    root["edge-tip-material"] = cfg.edge_tip_material;
    serialize_vec3(root["edge-tip-scale"], cfg.edge_tip_scale);

    auto& edge_type_val = root["edge-type"];
    edge_type_val["font-name"] = cfg.edge_type_font_name;
    edge_type_val["char-height"] = cfg.edge_type_char_height;
    serialize_rgb(edge_type_val["color"], cfg.edge_type_color);
    edge_type_val["space-width"] = cfg.edge_type_space_width;

    BOOST_LOG_TRIVIAL(debug) << "serialized rendering graph";
}

auto serialize_minimap(json_val& root, const minimap_config& cfg) -> void
{
    root["left"] = cfg.left;
    root["top"] = cfg.top;
    root["right"] = cfg.right;
    root["bottom"] = cfg.bottom;
    serialize_rgb(root["background-color"], cfg.background_col);
    root["zoom-out"] = cfg.zoom_out;
    root["render-shadows"] = cfg.render_shadows;
    root["render-sky"] = cfg.render_sky;
    root["render-vertices"] = cfg.render_vertices;
    root["render-vertex-ids"] = cfg.render_vertex_ids;
    root["render-edges"] = cfg.render_edges;
    root["render-edge-types"] = cfg.render_edge_types;
    root["render-edge-tips"] = cfg.render_edge_tips;
    root["render-particles"] = cfg.render_particles;

    BOOST_LOG_TRIVIAL(debug) << "serialized rendering minimap";
}

auto serialize(json_val& root, const config_data& cfg) -> void
{
    serialize_background(root["background"], cfg.background);
    serialize_graph(root["graph"], cfg.graph);
    serialize_minimap(root["minimap"], cfg.minimap);

    BOOST_LOG_TRIVIAL(debug) << "serialized rendering";
}

} // namespace rendering