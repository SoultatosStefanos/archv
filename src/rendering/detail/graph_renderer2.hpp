// Contains a private module for the graph renderer class.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_GRAPH_RENDERER2_HPP
#define RENDERING_DETAIL_GRAPH_RENDERER2_HPP

#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <optional>
#include <unordered_map>

// fwd declare cause this implementation is not exported by Ogre.
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
    using position_type = Ogre::Vector3;
    using scale_type = Ogre::Vector3;

    id_type id;
    position_type pos;
    std::optional< scale_type > scale;

    auto operator==(const vertex_properties&) const -> bool = default;
    auto operator!=(const vertex_properties&) const -> bool = default;
};

/***********************************************************
 * Edge Properties                                         *
 ***********************************************************/

struct edge_properties
{
    using dependency_type = std::string;
    using weight_type = int;

    vertex_properties source;
    vertex_properties target;
    dependency_type dependency;

    std::optional< weight_type > weight;

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

    using id_type = vertex_properties::id_type;
    using position_type = vertex_properties::position_type;
    using scale_type = vertex_properties::scale_type;

    using dependency_type = edge_properties::dependency_type;
    using weight_type = edge_properties::weight_type;

    explicit graph_renderer_impl(
        scene_type& scene,
        const config_data_type& cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT);

    graph_renderer_impl(const graph_renderer_impl&) = default;
    graph_renderer_impl(graph_renderer_impl&&) = delete;

    ~graph_renderer_impl();

    auto operator=(const graph_renderer_impl&)
        -> graph_renderer_impl& = default;
    auto operator=(graph_renderer_impl&&) -> graph_renderer_impl& = delete;

    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

    auto config_data() const -> const config_data_type& { return *m_cfg; }

    auto resource_group() const -> auto* { return m_resource_group.data(); }

    auto setup_vertex(id_type v, position_type pos) -> void;
    auto setup_edge(
        const id_type& source,
        const id_type& target,
        dependency_type dependency) -> void;

    auto shutdown_vertex(const id_type& v) -> void;
    auto shutdown_edge(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency) -> void;

    auto render_vertex_pos(const id_type& v, position_type pos) -> void;
    auto render_edge_pos(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency) -> void;

    auto render_vertex_scaling(const id_type& v, scale_type scale) -> void;
    auto render_edge_scaling(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency) -> void;

    auto hide_vertex_scaling(const id_type& v) -> void;
    auto hide_edge_scaling(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency) -> void;

    auto draw_vertex(const id_type& v, const config_data_type& cfg) -> void;
    auto draw_vertex(const id_type& v, config_data_type&& cfg) -> void = delete;

    auto draw_edge(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency,
        const config_data_type& cfg) -> void;
    auto draw_edge(
        const id_type& source,
        const id_type& target,
        const dependency_type& dependency,
        config_data_type&& cfg) -> void
        = delete;

private:
    using vertex_map
        = std::unordered_map< vertex_properties::id_type, vertex_properties >;
    using edge_map = std::unordered_map< std::string, edge_properties >;

    using vertex_text_map = std::unordered_map<
        vertex_properties::id_type,
        std::unique_ptr< Ogre::MovableText > >;

    using edge_text_map = std::
        unordered_map< std::string, std::unique_ptr< Ogre::MovableText > >;

    scene_type& m_scene;
    const config_data_type* m_cfg { nullptr };
    std::string_view m_resource_group;

    vertex_map m_vertices;
    edge_map m_edges;
    vertex_text_map m_vertex_texts;
    edge_text_map m_edge_texts;
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER2_HPP
