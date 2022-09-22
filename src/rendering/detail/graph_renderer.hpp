// Contains a private module for the graph renderer class.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_GRAPH_RENDERER_HPP
#define RENDERING_DETAIL_GRAPH_RENDERER_HPP

#include <OGRE/OgreSceneManager.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Ogre
{
class MovableText;
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

struct vertex_properties
{
    using id_type = std::string;

    id_type id;

    auto operator==(const vertex_properties&) const -> bool = default;
    auto operator!=(const vertex_properties&) const -> bool = default;
};

/***********************************************************
 * Edge Properties                                         *
 ***********************************************************/

struct edge_properties
{
    vertex_properties source;
    vertex_properties target;

    auto operator==(const edge_properties&) const -> bool = default;
    auto operator!=(const edge_properties&) const -> bool = default;
};

/***********************************************************
 * Graph Renderer Implementation                           *
 ***********************************************************/

class graph_renderer_impl
{
public:
    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config;

    using position_type = Ogre::Vector3;

    explicit graph_renderer_impl(scene_type& scene);
    ~graph_renderer_impl();

    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

    void draw(const vertex_properties& v, const config_data_type& cfg);
    void draw(const edge_properties& e, const config_data_type& cfg);

    auto erase(const vertex_properties& v) -> void;
    auto erase(const edge_properties& e) -> void;

    void draw_layout(const vertex_properties& v, const position_type& p);
    auto draw_layout(const edge_properties& e) -> void;

    // TODO Edge Dependency, Vertex Cluster, etc.

private:
    using id_billboard = Ogre::MovableText;
    using id_cache = std::unordered_map<
        vertex_properties::id_type,
        std::unique_ptr< id_billboard > >;

    void
    draw_id_billboard(const vertex_properties& v, const config_data_type& cfg);

    scene_type& m_scene;

    mutable id_cache m_id_billboards;
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
