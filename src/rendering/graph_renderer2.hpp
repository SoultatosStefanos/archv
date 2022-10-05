// Contains a class responsible for rendering any architecture graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER2_HPP
#define RENDERING_GRAPH_RENDERER2_HPP

#include "detail/graph_renderer2.hpp"

#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <boost/graph/adjacency_list.hpp>
#include <concepts>
#include <string_view>

namespace rendering
{

/***********************************************************
 * Graph config                                            *
 ***********************************************************/

struct graph_config
{
    Ogre::String vertex_mesh;
    Ogre::Vector3 vertex_scale;
    Ogre::String vertex_id_font_name;
    Ogre::Real vertex_id_char_height;
    Ogre::ColourValue vertex_id_color;
    Ogre::Real vertex_id_space_width;

    Ogre::String edge_material;
    Ogre::String edge_tip_mesh;
    Ogre::Vector3 edge_tip_scale;

#if (1) // FIXME
    Ogre::String edge_type_font_name { "Roboto-Medium" };
    Ogre::Real edge_type_char_height { 4.0 };
    Ogre::ColourValue edge_type_color { 0.2, 0.2, 0.2 };
    Ogre::Real edge_type_space_width { 1.0 };
#endif

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

/***********************************************************
 * Graph renderer                                          *
 ***********************************************************/

// Generic directed graph renderer.
// Will prepare a scene at a render window upon initialization.
// NOTE: Parallel edges are allowed.
template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    typename PositionMap >
class graph_renderer
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            VertexID,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            DependencyMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            PositionMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;

public:
    using graph_type = Graph;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;
    using vertex_id_type = VertexID;
    using dependency_map_type = DependencyMap;
    using position_map_type = PositionMap;

    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config_data;
    using config_api_type = graph_config_api;

    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        dependency_map_type edge_dependency,
        position_map_type vertex_pos,
        scene_type& scene,
        config_data_type cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT)
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_edge_dependency { edge_dependency }
    , m_vertex_pos { vertex_pos }
    , m_scene { scene }
    , m_cfg { cfg }
    , m_defaults { cfg }
    , m_cfg_api { std::move(cfg) }
    , m_impl { scene(), config_data(), resource_group }
    {
        visit_vertices([this](auto v)
                       { m_impl.setup(make_vertex_properties(v)); });

        visit_edges([this](auto e) { m_impl.setup(make_edge_properties(e)); });
    }

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = delete;

    ~graph_renderer()
    {
        visit_edges([this](auto e)
                    { m_impl.shutdown(make_edge_properties(e)); });

        visit_vertices([this](auto v)
                       { m_impl.shutdown(make_vertex_properties(v)); });
    }

    auto operator=(const graph_renderer&) -> graph_renderer& = default;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto graph() const -> const graph_type& { return m_g; }
    auto vertex_id() const -> vertex_id_type { return m_vertex_id; }
    auto edge_dependency() const -> dependency_map { return m_edge_dependency; }
    auto vertex_position() const -> position_map_type { return m_vertex_pos; }

    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

    auto default_data() const -> const config_data_type& { return m_defaults; }

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto config_api() const -> const config_api_type& { return m_cfg_api; }
    auto config_api() -> config_api_type& { return m_cfg_api; }

    auto resource_group() const -> auto* { return m_impl.resource_group(); }

    inline auto render_layout() -> void
    {
        visit_vertices([this](auto v)
                       { m_impl.render_pos(make_vertex_properties(v)); });

        visit_edges([this](auto e)
                    { m_impl.render_pos(make_edge_properties(e)); });
    }

    template < typename WeightMap >
    inline auto render_weights(WeightMap edge_weight) -> void
    {
        visit_edges(
            [this, edge_weight](auto e)
            { m_impl.render_weight(make_edge_properties(e, edge_weight)); });
    }

    template < typename ScaleMap >
    inline auto render_scaling(ScaleMap vertex_scale) -> void
    {
        visit_vertices(
            [this, vertex_scale](auto v) {
                m_impl.render_scaling(make_vertex_properties(v, vertex_scale));
            });

        visit_edges([this](auto e)
                    { m_impl.render_scaling(make_edge_properties(e)); });
    }

    inline auto hide_weights() -> void
    {
        visit_edges([this](auto e)
                    { m_impl.hide_weight(make_edge_properties(e)); });
    }

    inline auto hide_scaling() -> void
    {
        visit_vertices([this](auto v)
                       { m_impl.hide_scaling(make_vertex_properties(v)); });

        visit_edges([this](auto e)
                    { m_impl.hide_scaling(make_edge_properties(e)); });
    }

    inline auto draw(const config_data_type& cfg) -> void
    {
        visit_vertices([this, &cfg](auto v)
                       { m_impl.draw(make_vertex_properties(v), cfg); });

        visit_edges([this, &cfg](auto e)
                    { m_impl.draw(make_edge_properties(e), cfg); });
    }

    auto draw(config_data_type&&) -> void = delete; // disallow temporaries

protected:
    template < typename UnaryOperation >
    requires std::invocable< UnaryOperation, vertex_type >
    auto visit_vertices(UnaryOperation f) const
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            f(v);
    }

    template < typename UnaryOperation >
    requires std::invocable< UnaryOperation, edge_type >
    auto visit_edges(UnaryOperation f) const
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            f(e);
    }

private:
    using impl_type = detail::graph_renderer_impl;
    using vertex_properties = detail::vertex_properties;
    using edge_properties = detail::edge_properties;

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< vertex_id_type >::value_type,
                  vertex_properties::id_type >);

    inline auto make_vertex_properties(vertex_type v) const
    {
        using namespace Ogre;

        auto&& id = boost::get(vertex_id(), v);
        const auto& [x, y, z] = boost::get(vertex_position(), v);

        return vertex_properties { .id = std::move(id),
                                   .pos = Vector3(x, y, z) };
    }

    template < typename ScaleMap >
    inline auto
    make_vertex_properties(vertex_type v, ScaleMap vertex_scale) const
    {
        using namespace Ogre;

        auto&& id = boost::get(vertex_id(), v);
        const auto& [x, y, z] = boost::get(vertex_position(), v);
        const auto& [x1, y1, z1] = boost::get(vertex_scale, v);

        return vertex_properties { .id = std::move(id),
                                   .pos = Vector3(x, y, z),
                                   .scale = Vector3(x1, y1, z1) };
    }

    inline auto make_edge_properties(edge_type e) const
    {
        auto&& dependency = boost::get(edge_dependency(), e);
        const auto src = boost::source(e, graph());
        const auto trgt = boost::target(e, graph());

        return edge_properties { .dependency = std::move(dependency),
                                 .source = make_vertex_properties(src),
                                 .target = make_vertex_properties(trgt) };
    }

    template < typename WeightMap >
    inline auto make_edge_properties(edge_type e, WeightMap edge_weight) const
    {
        auto&& dependency = boost::get(edge_dependency(), e);
        const auto src = boost::source(e, graph());
        const auto trgt = boost::target(e, graph());
        const auto weight = boost::get(edge_weight, e);

        return edge_properties { .dependency = std::move(dependency),
                                 .source = make_vertex_properties(src),
                                 .target = make_vertex_properties(trgt),
                                 .weight = weight };
    }

    const graph_type& m_g;
    vertex_id_type m_vertex_id;
    dependency_map_type m_edge_dependency;
    position_map_type m_vertex_pos;

    scene_type& m_scene;
    config_data_type m_cfg, m_defaults;
    config_api_type m_cfg_api;

    impl_type m_impl;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER2_HPP
