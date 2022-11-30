// Contains a config module for the interactive properties of the minimap.
// Soultatos Stefanos 2022

#ifndef RENDERING_MINIMAP_CONFIG_HPP
#define RENDERING_MINIMAP_CONFIG_HPP

#include <OGRE/Ogre.h>

namespace rendering
{

/***********************************************************
 * Minimap Config                                          *
 ***********************************************************/

struct minimap_config
{
    using coord_type = Ogre::Real;
    using rgba_type = Ogre::ColourValue;
    using dist_type = Ogre::Real;

    coord_type left, top, right, bottom;
    rgba_type background_col;
    dist_type zoom_out; // Relative to the main camera
    bool render_shadows;
    bool render_sky;
    bool render_vertices;
    bool render_vertex_ids;
    bool render_edges;
    bool render_edge_types;
    bool render_edge_tips;
    bool render_particles;

    auto operator==(const minimap_config&) const -> bool = default;
    auto operator!=(const minimap_config&) const -> bool = default;
};

/***********************************************************
 * Minimap Config API                                      *
 ***********************************************************/

class minimap_config_api
{
public:
    using config_data_type = minimap_config;
    using coord_type = config_data_type::coord_type;
    using dist_type = config_data_type::dist_type;
    using rgba_type = config_data_type::rgba_type;

    explicit minimap_config_api(config_data_type cfg);

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto set_left(coord_type c) -> void;
    auto set_top(coord_type c) -> void;
    auto set_right(coord_type c) -> void;
    auto set_bottom(coord_type c) -> void;
    auto set_zoom_out(dist_type dist) -> void;
    auto set_background_color(rgba_type col) -> void;
    auto set_render_shadows(bool v) -> void;
    auto set_render_sky(bool v) -> void;
    auto set_render_vertices(bool v) -> void;
    auto set_render_vertex_ids(bool v) -> void;
    auto set_render_edges(bool v) -> void;
    auto set_render_edge_types(bool v) -> void;
    auto set_render_edge_tips(bool v) -> void;
    auto set_render_particles(bool v) -> void;

private:
    config_data_type m_cfg;
};

} // namespace rendering

#endif // RENDERING_MINIMAP_CONFIG_HPP
