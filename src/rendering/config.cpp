#include "config.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>
#include <string>

namespace rendering
{

namespace
{
    template < typename T >
    auto deserialize_triad(const Json::Value& val)
    {
        auto extract_val = [iter = std::begin(val)]() mutable -> T
        {
            auto val = (*iter).as< T >();
            std::advance(iter, 1);
            return val;
        };

        auto&& f = extract_val();
        auto&& s = extract_val();
        auto&& t = extract_val();

        return std::make_tuple(std::move(f), std::move(s), std::move(t));
    }

    inline auto deserialize_rgb(const Json::Value& val) -> Ogre::ColourValue
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

    auto deserialize_background(const Json::Value& val) -> background_config
    {
        using real = double;
        using string = std::string;

        static_assert(std::is_convertible_v< real, Ogre::Real >);
        static_assert(std::is_convertible_v< string, Ogre::String >);

        auto&& skybox_material = val["skybox-material"].as< string >();
        auto skybox_dist = val["skybox-distance"].as< real >();
        auto&& ambient_light = deserialize_rgb(val["ambient-light"]);
        auto&& diffuse_light = deserialize_rgb(val["diffuse-light"]);
        auto&& specular_light = deserialize_rgb(val["specular-light"]);
        auto near_clip_dist = val["near-clip-distance"].as< real >();
        auto far_clip_dist = val["far-clip-distance"].as< real >();

        BOOST_LOG_TRIVIAL(debug) << "deserialized rendering background";

        return { std::move(skybox_material),
                 static_cast< Ogre::Real >(skybox_dist),
                 ambient_light,
                 diffuse_light,
                 specular_light,
                 static_cast< Ogre::Real >(near_clip_dist),
                 static_cast< Ogre::Real >(far_clip_dist) };
    }

    inline auto deserialize_vector3(const Json::Value& val) -> Ogre::Vector3
    {
        using real = double;

        const auto [xs, ys, zs] = deserialize_triad< real >(val);

        BOOST_LOG_TRIVIAL(debug) << "deserialized vector3: (" << xs << ", "
                                 << ys << ", " << zs << ')';

        return Ogre::Vector3(xs, ys, zs);
    }

    template < typename T >
    inline auto deserialize_degree_ranks(const Json::Value& val)
        -> std::array< T, vertex_degree_effects::num_ranks >
    {
        auto&& [light, medium, heavy] = deserialize_triad< T >(val);

        return { std::move(light), std::move(medium), std::move(heavy) };
    }

    inline auto deserialize_degree_effects(const Json::Value& val)
        -> vertex_degree_effects
    {
        using threshold_type = vertex_degree_effects::threshold_type;

        auto&& thresholds
            = deserialize_degree_ranks< threshold_type >(val["thresholds"]);
        auto&& particle_systems
            = deserialize_degree_ranks< Ogre::String >(val["particle-systems"]);

        BOOST_LOG_TRIVIAL(debug) << "deserialized degree effects";

        return vertex_degree_effects { .thresholds = thresholds,
                                       .particle_systems = particle_systems };
    }

    auto deserialize_graph(const Json::Value& val) -> graph_config
    {
        using string = std::string;
        using real = double;

        static_assert(std::is_convertible_v< string, Ogre::String >);

        auto&& vertex_mesh = val["vertex-mesh"].as< string >();
        auto&& vertex_scale = deserialize_vector3(val["vertex-scale"]);

        const auto& vboard_val = val["vertex-id"];
        auto&& vboard_font_name = vboard_val["font-name"].as< string >();
        auto vboard_char_height = vboard_val["char-height"].as< real >();
        auto&& vboard_color = deserialize_rgb(vboard_val["color"]);
        auto vboard_space_width = vboard_val["space-width"].as< real >();

        auto&& edge_material = val["edge-material"].as< string >();
        auto&& edge_tip_mesh = val["edge-tip-mesh"].as< string >();
        auto&& edge_tip_scale = deserialize_vector3(val["edge-tip-scale"]);

        const auto& edgetype_val = val["edge-type"];
        auto&& edgetype_font_name = edgetype_val["font-name"].as< string >();
        auto edgetype_char_height = edgetype_val["char-height"].as< real >();
        auto&& edgetype_color = deserialize_rgb(edgetype_val["color"]);
        auto edgetype_space_width = edgetype_val["space-width"].as< real >();

        const auto& degree_effects_val = val["degree-effects"];
        auto&& in_degree_effects
            = deserialize_degree_effects(degree_effects_val["in-degree"]);
        auto&& out_degree_effects
            = deserialize_degree_effects(degree_effects_val["out-degree"]);

        BOOST_LOG_TRIVIAL(debug) << "deserialized rendering graph";

        return { std::move(vertex_mesh),
                 vertex_scale,
                 std::move(vboard_font_name),
                 static_cast< float >(vboard_char_height),
                 vboard_color,
                 static_cast< float >(vboard_space_width),
                 std::move(edge_material),
                 std::move(edge_tip_mesh),
                 edge_tip_scale,
                 std::move(edgetype_font_name),
                 static_cast< float >(edgetype_char_height),
                 edgetype_color,
                 static_cast< float >(edgetype_space_width),
                 std::move(in_degree_effects),
                 std::move(out_degree_effects) };
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& bkg = deserialize_background(root["background"]);
    auto&& g = deserialize_graph(root["graph"]);

    return config_data { .background = std::move(bkg), .graph = std::move(g) };
}

} // namespace rendering