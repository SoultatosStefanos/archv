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
    using position_type = Ogre::Vector3;

    id_type id;
    position_type pos;

    auto operator==(const vertex_properties&) const -> bool = default;
    auto operator!=(const vertex_properties&) const -> bool = default;
};

/***********************************************************
 * Edge Properties                                         *
 ***********************************************************/

struct edge_properties
{
    using weight_type = int;

    vertex_properties source;
    vertex_properties target;
    weight_type weight;

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

    explicit graph_renderer_impl(scene_type& scene);
    ~graph_renderer_impl();

    auto draw(const vertex_properties& v, const config_data_type& cfg) -> void;
    auto draw(const edge_properties& e, const config_data_type& cfg) -> void;

    auto draw_layout(const vertex_properties& v) -> void;
    auto draw_layout(const edge_properties& e) -> void;

    auto clear(const vertex_properties& v) -> void;
    auto clear(const edge_properties& e) -> void;

protected:
    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

private:
    using id_billboard = Ogre::MovableText;
    using id_cache = std::unordered_map<
        vertex_properties::id_type,
        std::unique_ptr< id_billboard > >;

    auto redraw(const vertex_properties& v, const config_data_type& c) -> void;

    auto draw_id(const vertex_properties& v, const config_data_type& c) -> void;
    auto redraw_id(const vertex_properties&, const config_data_type&) -> void;

    auto redraw(const edge_properties& e, const config_data_type& c) -> void;

    auto draw_tip(const edge_properties& e, const config_data_type& c) -> void;
    auto redraw_tip(const edge_properties& e, const config_data_type&) -> void;

    scene_type& m_scene;

    mutable id_cache m_id_billboards; // for cleanup
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
