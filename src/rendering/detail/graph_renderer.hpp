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
struct graph_config;
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

    graph_renderer_impl(
        Ogre::SceneManager* scene, const config_data_type& config);

    ~graph_renderer_impl();

    auto setup_vertex(const vertex_rendering_properties& v) const -> void;
    auto setup_edge(const edge_rendering_properties& e) const -> void;

    auto shutdown_vertex(const vertex_rendering_properties& v) const -> void;
    auto shutdown_edge(const edge_rendering_properties& e) const -> void;

    auto update_vertex(const vertex_rendering_properties& v) const -> void;
    auto update_edge(const edge_rendering_properties& e) const -> void;

protected:
    auto scene() const -> auto* { return m_scene; }

    auto config_data() const -> const auto& { return m_config; }
    auto config_data() -> auto& { return m_config; }

private:
    using id_billboards = std::
        unordered_map< Ogre::String, std::unique_ptr< Ogre::MovableText > >;

    auto setup_id_billboard(const vertex_rendering_properties& v) const -> void;
    auto shutdown_id_billboard(const vertex_rendering_properties& v) const
        -> void;

    Ogre::SceneManager* m_scene { nullptr };
    const config_data_type& m_config;

    mutable id_billboards m_vertices_billboards;
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
