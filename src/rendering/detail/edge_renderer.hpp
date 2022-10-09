// Contains a private edge renderer implementation.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_EDGE_RENDERER_HPP
#define RENDERING_DETAIL_EDGE_RENDERER_HPP

#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <memory>
#include <set>
#include <unordered_map>

/***********************************************************
 * Fwd Declarations                                        *
 ***********************************************************/

// fwd declare cause this implementation is not exported by Ogre.
namespace Ogre
{
class MovableText;
} // namespace Ogre

namespace Procedural
{
class Path;
} // namespace Procedural

namespace rendering
{
struct graph_config;
} // namespace rendering

namespace rendering::detail
{

struct edge_properties;

/***********************************************************
 * Helpers                                                 *
 ***********************************************************/

// Compates edges (<) by taking into account only their vertices.
struct edge_vertices_compare
{
    using edge_type = edge_properties;

    auto operator()(const edge_type& lhs, const edge_type& rhs) const -> bool;
    auto operator()(const edge_type* lhs, const edge_type* rhs) const -> bool;
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

    edge_renderer(
        scene_type& scene,
        const config_data_type& cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT);

    edge_renderer(const edge_renderer&) = default;
    edge_renderer(edge_renderer&&) = delete;

    ~edge_renderer();

    auto operator=(const edge_renderer&) -> edge_renderer& = default;
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

    using path_type = Procedural::Path;

    using parallels_set
        = std::multiset< const edge_type*, edge_vertices_compare >;

    auto edge(const name_type& name) const -> const edge_type&;
    auto edge(const name_type& name) -> edge_type&;

    auto edge_txt(const name_type& name) const -> const edge_txt_type&;
    auto edge_txt(const name_type& name) -> edge_txt_type&;

    auto is_parallel(const edge_type& e) const -> bool;
    auto is_first_parallel(const edge_type& e) const -> bool;
    auto first_parallel(const edge_type& e) const -> const edge_type&;

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

#endif // RENDERING_DETAIL_EDGE_RENDERER_HPP
