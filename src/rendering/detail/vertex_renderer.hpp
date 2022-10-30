// Contains a private vertex renderer implementation class.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_VERTEX_RENDERER_HPP
#define RENDERING_DETAIL_VERTEX_RENDERER_HPP

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

namespace rendering::detail
{

struct vertex_properties;

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

    auto hide_scale(const id_type& id) -> void;

    auto render_in_degree_particles(
        const id_type& id, const std::optional< name_type >& particles) -> void;
    auto render_out_degree_particles(
        const id_type&, const std::optional< name_type >& particles) -> void;

    auto render_cluster(const id_type& id, const rgba_type& col) -> void;

    // TODO
    // TODO Clean up solid colors
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

} // namespace rendering::detail

#endif // RENDERING_DETAIL_VERTEX_RENDERER_HPP
