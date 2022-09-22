// Contains a class responsible for rendering any boost::graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include "detail/graph_renderer.hpp"

#include <OGRE/OgreSceneManager.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>

namespace rendering
{

/***********************************************************
 * Graph config                                            *
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
    Ogre::String edge_tip_mesh = "triangle.mesh";
    Ogre::Vector3 edge_tip_scale = { 0.5, 0.5, 0.5 };

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

// Can render any boost::graph, given that every node has an id.
template < typename Graph, typename VertexID >
class graph_renderer
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            VertexID,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< VertexID >::value_type,
                  detail::vertex_properties::id_type >);

public:
    /***********************************************************
     * Member types                                            *
     ***********************************************************/

    using graph_type = Graph;
    using vertex_id_type = VertexID;
    using vertex_type =
        typename boost::graph_traits< graph_type >::vertex_descriptor;
    using edge_type =
        typename boost::graph_traits< graph_type >::edge_descriptor;

    using scene_type = Ogre::SceneManager;

    using config_data_type = graph_config;
    using config_api_type = graph_config_api;

    /***********************************************************
     * Member functions                                        *
     ***********************************************************/

    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        scene_type& scene,
        config_data_type cfg = config_data_type())
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_scene { scene }
    , m_config { cfg }
    , m_defaults { cfg }
    , m_config_api { std::move(cfg) }
    , m_impl { scene }
    {
    }

    ~graph_renderer() { clear(); }

    graph_renderer(const graph_renderer&) = delete;
    graph_renderer(graph_renderer&&) = delete;

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    /***********************************************************
     * Member access                                           *
     ***********************************************************/

    auto graph() const -> const graph_type& { return m_g; }
    auto vertex_id() const -> vertex_id_type { return m_vertex_id; }
    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

    auto default_data() const -> const config_data_type& { return m_defaults; }
    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }
    auto config_api() const -> const config_api_type& { return m_config_api; }
    auto config_api() -> config_api_type& { return m_config_api; }

    /***********************************************************
     * (Scene) Modifiers                                       *
     ***********************************************************/

    inline auto draw(const config_data_type& cfg) -> void
    {
        visit_vertices([this, &cfg](auto v)
                       { m_impl.draw(make_vertex_properties(v), cfg); });

        visit_edges([this, &cfg](auto e)
                    { m_impl.draw(make_edge_properties(e), cfg); });
    }

    inline auto clear() -> void
    {
        visit_edges([this](auto e) { m_impl.erase(make_edge_properties(e)); });

        visit_vertices([this](auto v)
                       { m_impl.erase(make_vertex_properties(v)); });
    }

    template < typename PositionMap >
    inline auto draw_layout(PositionMap vertex_position) -> void
    {
        BOOST_CONCEPT_ASSERT(
            (boost::ReadablePropertyMapConcept< PositionMap, vertex_type >));

        visit_vertices(
            [this, vertex_position](auto v)
            {
                const auto& [x, y, z] = boost::get(vertex_position, v);
                const auto pos = graph_renderer_impl::position_type(x, y, z);
                m_impl.draw_layout(make_vertex_properties(v), pos);
            });

        visit_edges([this](auto e)
                    { m_impl.draw_layout(make_edge_properties(e)); });
    }

    // TODO Edge Dependency draw, Vertex Cluster draw, etc.

private:
    using graph_renderer_impl = detail::graph_renderer_impl;
    using vertex_properties = detail::vertex_properties;
    using edge_properties = detail::edge_properties;

    inline auto make_vertex_properties(vertex_type v) const
    {
        auto&& id = boost::get(vertex_id(), v);
        return vertex_properties { .id = std::move(id) };
    }

    inline auto make_edge_properties(edge_type e) const
    {
        const auto source = boost::source(e, graph());
        const auto target = boost::target(e, graph());
        return edge_properties { .source = make_vertex_properties(source),
                                 .target = make_vertex_properties(target) };
    }

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

    const graph_type& m_g;
    vertex_id_type m_vertex_id;
    scene_type& m_scene;
    config_data_type m_config, m_defaults;
    config_api_type m_config_api;

    graph_renderer_impl m_impl;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
