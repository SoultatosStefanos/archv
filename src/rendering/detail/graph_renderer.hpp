// Contains two private vertex/edge renderers.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_GRAPH_RENDERER_HPP
#define RENDERING_DETAIL_GRAPH_RENDERER_HPP

#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <memory>
#include <optional>
#include <unordered_map>

/***********************************************************
 * Fwd Declarations                                        *
 ***********************************************************/

// fwd declare cause this implementation is not exported by Ogre.
namespace Ogre
{
class MovableText;
} // namespace Ogre

namespace rendering
{
struct graph_config;
} // namespace rendering

namespace Procedural
{
class Path;
} // namespace Procedural

namespace rendering::detail
{

struct vertex_properties;
struct edge_properties;

/***********************************************************
 * Helpers                                                 *
 ***********************************************************/

// Compares edges (<) by taking into account only their vertices.
struct edge_vertices_compare
{
    using edge_type = edge_properties;

    auto operator()(const edge_type& lhs, const edge_type& rhs) const -> bool;
    auto operator()(const edge_type* lhs, const edge_type* rhs) const -> bool;
};

/***********************************************************
 * Vertex Renderer                                         *
 ***********************************************************/

// Exposes an api for rendering/updating architecture graph vertices.
// NOTE: Will prepare a scene on construction.
class vertex_renderer
{
public:
    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config;

    using id_type = std::string;
    using position_type = Ogre::Vector3;
    using scale_type = Ogre::Vector3;
    using degree_type = int;

    using name_type = std::string;
    using rgba_type = Ogre::ColourValue;

    vertex_renderer(
        scene_type& scene,
        const config_data_type& cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT);

    vertex_renderer(const vertex_renderer&) = default;
    vertex_renderer(vertex_renderer&&) = default;

    ~vertex_renderer();

    auto operator=(const vertex_renderer&) -> vertex_renderer& = delete;
    auto operator=(vertex_renderer&&) -> vertex_renderer& = delete;

    auto setup(const id_type& id, position_type pos) -> void;
    auto shutdown(const id_type& id) -> void;

    auto render_position(const id_type& id, position_type pos) -> void;

    auto render_scale(const id_type& id, scale_type scale) -> void;

    auto render_in_degree_particles(
        const id_type& id, const std::optional< name_type >& particles) -> void;
    auto render_out_degree_particles(
        const id_type&, const std::optional< name_type >& particles) -> void;

    auto render_cluster(const id_type& id, const rgba_type& col) -> void;

    auto hide_scale(const id_type& id) -> void;

    auto hide_cluster(const id_type& id) -> void;

    auto draw(const id_type& id, const config_data_type& cfg) -> void;
    auto draw(const id_type&, config_data_type&&) -> void = delete;

private:
    using vertex_type = vertex_properties;
    using vertex_txt_type = Ogre::MovableText;
    using vertex_ptr = std::unique_ptr< vertex_properties >;
    using text_ptr = std::unique_ptr< Ogre::MovableText >;
    using vertex_map = std::unordered_map< id_type, vertex_ptr >;
    using vertex_text_map = std::unordered_map< id_type, text_ptr >;

    auto vertex(const id_type& id) const -> const vertex_type&;
    auto vertex(const id_type& id) -> vertex_type&;

    auto vertex_txt(const id_type& id) const -> const vertex_txt_type&;
    auto vertex_txt(const id_type& id) -> vertex_txt_type&;

    auto setup_model(const vertex_type& v) -> void;
    auto setup_text(const vertex_type& v) -> void;

    auto shutdown_text(const vertex_type& v) -> void;
    auto shutdown_model(const vertex_type& v) -> void;

    // effect name is specific to the vertex, in order to be rendered with this
    // as an "id".
    auto render_degree_particles(
        const id_type& id,
        const std::optional< name_type >& particle_system,
        std::optional< name_type >& curr_effect,
        name_type&& new_effect) -> void;

    auto shutdown_in_degree_particles(const id_type& id) -> void;
    auto shutdown_out_degree_particles(const id_type& id) -> void;
    auto shutdown_degree_particles(std::optional< name_type >& curr) -> void;

    auto draw_model(const vertex_type& v) -> void;
    auto draw_scale(const vertex_type& v) -> void;
    auto draw_text(const vertex_type& v) -> void;

    scene_type& m_scene;
    const config_data_type* m_cfg { nullptr };
    const char* m_resource_group { nullptr };

    vertex_map m_vertices;
    vertex_text_map m_vertex_texts;
};

/***********************************************************
 * Edge Renderer                                           *
 ***********************************************************/

// Exposes an api for rendering/updating architecture graph edges.
// NOTE: Will prepare a scene on construction.
class edge_renderer
{
public:
    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config;

    using vertex_id_type = std::string;
    using dependency_type = std::string;
    using weight_type = int;

    using rgba_type = Ogre::ColourValue;

    edge_renderer(
        scene_type& scene,
        const config_data_type& cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT);

    edge_renderer(const edge_renderer&) = default;
    edge_renderer(edge_renderer&&) = default;

    ~edge_renderer();

    auto operator=(const edge_renderer&) -> edge_renderer& = delete;
    auto operator=(edge_renderer&&) -> edge_renderer& = delete;

    auto setup(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency) -> void;

    auto shutdown(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency) -> void;

    auto render_position(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency) -> void;

    auto render_weight(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency,
        weight_type weight) -> void;

    auto render_cluster(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency,
        const rgba_type& col) -> void;

    auto hide_weight(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency) -> void;

    auto hide_cluster(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency) -> void;

    auto draw(
        const vertex_id_type& source,
        const vertex_id_type& target,
        const dependency_type& dependency,
        const config_data_type& cfg) -> void;

    auto draw(
        const vertex_id_type&,
        const vertex_id_type&,
        const dependency_type&,
        config_data_type&&) -> void
        = delete;

private:
    using edge_type = edge_properties;
    using name_type = std::string;
    using edge_txt_type = Ogre::MovableText;
    using edge_ptr = std::unique_ptr< edge_type >;
    using text_ptr = std::unique_ptr< Ogre::MovableText >;
    using edge_map = std::unordered_map< name_type, edge_ptr >;
    using edge_text_map = std::unordered_map< name_type, text_ptr >;
    using dependencies = std::vector< dependency_type >;

    using path_type = Procedural::Path;

    using parallels_set
        = std::multiset< const edge_type*, edge_vertices_compare >;

    using parallels_caption = std::pair< std::string, weight_type >;

    auto edge(const name_type& name) const -> const edge_type&;
    auto edge(const name_type& name) -> edge_type&;

    auto edge_txt(const name_type& name) const -> const edge_txt_type&;
    auto edge_txt(const name_type& name) -> edge_txt_type&;

    auto is_parallel(const edge_type& e) const -> bool;
    auto is_first_parallel(const edge_type& e) const -> bool;
    auto first_parallel(const edge_type& e) const -> const edge_type&;
    auto make_parallels_caption(const edge_type& e) const -> std::string;
    auto make_parallels_weighted_caption(const edge_type& e) const
        -> parallels_caption;

    auto setup_model(const edge_type& e, const path_type& path) -> void;
    auto setup_tip(const edge_type& e, const path_type& path) -> void;
    auto setup_text(const edge_type& e, const path_type& path) -> void;

    auto shutdown_text(const edge_type& e) -> void;
    auto shutdown_tip(const edge_type& e) -> void;
    auto shutdown_model(const edge_type& e) -> void;

    auto render_model_pos(const edge_type& e, const path_type& path) -> void;
    auto render_tip_pos(const edge_type& e, const path_type& path) -> void;
    auto render_text_pos(const edge_type& e, const path_type& path) -> void;

    auto draw_model(const edge_type& e, const path_type& path) -> void;
    auto draw_tip(const edge_type& e, const path_type& path) -> void;
    auto draw_text(const edge_type& e, const path_type& path) -> void;

    scene_type& m_scene;
    const config_data_type* m_cfg { nullptr };
    const char* m_resource_group { nullptr };

    edge_map m_edges;
    edge_text_map m_edge_texts;
    parallels_set m_parallels;
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
