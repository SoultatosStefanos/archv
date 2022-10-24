// Contains a configuration module for the graph rendering.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_CONFIG_HPP
#define RENDERING_GRAPH_CONFIG_HPP

#include <OGRE/Ogre.h>

namespace rendering
{

/***********************************************************
 * Graph config                                            *
 ***********************************************************/

struct graph_config
{
    using name_type = Ogre::String;
    using scale_type = Ogre::Vector3;
    using height_type = Ogre::Real;
    using width_type = Ogre::Real;
    using rgba_type = Ogre::ColourValue;

    name_type vertex_mesh;
    scale_type vertex_scale;
    name_type vertex_id_font_name;
    height_type vertex_id_char_height;
    rgba_type vertex_id_color;
    width_type vertex_id_space_width;

    name_type edge_material;
    name_type edge_tip_mesh;
    scale_type edge_tip_scale;
    name_type edge_type_font_name;
    height_type edge_type_char_height;
    rgba_type edge_type_color;
    width_type edge_type_space_width;

    auto operator==(const graph_config&) const -> bool = default;
    auto operator!=(const graph_config&) const -> bool = default;
};

/***********************************************************
 * Graph config api                                        *
 ***********************************************************/

class graph_config_api
{
public:
    using config_data_type = graph_config;
    using name_type = config_data_type::name_type;
    using scale_type = config_data_type::scale_type;
    using height_type = config_data_type::height_type;
    using width_type = config_data_type::width_type;
    using rgba_type = config_data_type::rgba_type;

    explicit graph_config_api(config_data_type cfg)
    : m_config { std::move(cfg) }
    {
    }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

    auto set_vertex_mesh(name_type name) -> void;
    auto set_vertex_scale(scale_type scale) -> void;
    auto set_vertex_id_font_name(name_type name) -> void;
    auto set_vertex_id_char_height(height_type h) -> void;
    auto set_vertex_id_color(rgba_type col) -> void;
    auto set_vertex_space_width(width_type w) -> void;

    auto set_edge_material(name_type name) -> void;
    auto set_edge_tip_mesh(name_type name) -> void;
    auto set_edge_tip_scale(scale_type scale) -> void;
    auto set_edge_type_font_name(name_type name) -> void;
    auto set_edge_type_char_height(height_type h) -> void;
    auto set_edge_type_color(rgba_type col) -> void;
    auto set_edge_type_space_width(width_type w) -> void;

private:
    config_data_type m_config;
};

} // namespace rendering

#endif // RENDERING_GRAPH_CONFIG_HPP
