// Contains a private module for the graph renderer class.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_GRAPH_RENDERER_HPP
#define RENDERING_DETAIL_GRAPH_RENDERER_HPP

#include <OGRE/OgreSceneManager.h>
#include <memory>
#include <unordered_map>

namespace Ogre
{
class MovableText; // fwd declare cause its not published in the main repo.
} // namespace Ogre

namespace rendering
{
/***********************************************************
 * Graph config data                                       *
 ***********************************************************/

struct graph_config
{
    Ogre::String vertex_mesh;
    Ogre::Vector3 vertex_scale;

    Ogre::String vbillboard_font_name;
    Ogre::Real vbillboard_char_height;
    Ogre::ColourValue vbillboard_color;
    Ogre::Real vbillboard_space_width;

    Ogre::String edge_material;

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

    explicit graph_config_api(config_data_type cfg)
    : m_config { std::move(cfg) }
    {
    }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

private:
    config_data_type m_config;
};
} // namespace rendering

namespace rendering::detail
{

/***********************************************************
 * Vertex Properties                                       *
 ***********************************************************/

struct vertex_rendering_properties
{
    const Ogre::String& id;
    const Ogre::Vector3 pos;
};

/***********************************************************
 * Edge Properties                                         *
 ***********************************************************/

struct edge_rendering_properties
{
    vertex_rendering_properties source;
    vertex_rendering_properties target;
};

/***********************************************************
 * Graph Renderer Implementation                           *
 ***********************************************************/

class graph_renderer_impl
{
public:
    using config_data_type = graph_config;
    using config_api_type = graph_config_api;

    graph_renderer_impl(Ogre::SceneManager* scene, config_data_type cfg);

    ~graph_renderer_impl();

    auto default_data() const -> const config_data_type& { return m_defaults; }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

    auto config_api() const -> const config_api_type& { return m_config_api; }
    auto config_api() -> config_api_type& { return m_config_api; }

    auto setup_vertex(const vertex_rendering_properties& v) const -> void;
    auto setup_vertex(
        const vertex_rendering_properties& v, const config_data_type& cfg) const
        -> void;
    auto setup_edge(const edge_rendering_properties& e) const -> void;

    auto shutdown_vertex(const vertex_rendering_properties& v) const -> void;
    auto shutdown_edge(const edge_rendering_properties& e) const -> void;

    auto update_vertex(const vertex_rendering_properties& v) const -> void;
    auto update_edge(const edge_rendering_properties& e) const -> void;

    auto draw_vertex(
        const vertex_rendering_properties& v, const config_data_type& cfg) const
        -> void;

    auto draw_edge(
        const edge_rendering_properties& e, const config_data_type& cfg) const
        -> void;

protected:
    auto scene() const -> auto* { return m_scene; }

private:
    using id_billboards = std::
        unordered_map< Ogre::String, std::unique_ptr< Ogre::MovableText > >;

    auto setup_id_billboard(const vertex_rendering_properties& v) const -> void;
    auto shutdown_id_billboard(const vertex_rendering_properties& v) const
        -> void;

    Ogre::SceneManager* m_scene { nullptr };
    config_data_type m_config, m_defaults;
    config_api_type m_config_api;

    mutable id_billboards m_vertices_billboards;
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
