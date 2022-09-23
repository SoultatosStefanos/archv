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
    Ogre::String vertex_id_font_name;
    Ogre::Real vertex_id_char_height;
    Ogre::ColourValue vertex_id_color;
    Ogre::Real vertex_id_space_width;

    Ogre::String edge_material;
    Ogre::String edge_tip_mesh;
    Ogre::Vector3 edge_tip_scale;

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

template <
    typename Graph,
    typename VertexID,
    typename PositionMap,
    typename WeightMap >
class graph_renderer
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            VertexID,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            PositionMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;

public:
    using graph_type = Graph;
    using vertex_id_type = VertexID;
    using position_map_type = PositionMap;
    using weight_map_type = WeightMap;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;

    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config;
    using config_api_type = graph_config_api;

    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        position_map_type vertex_pos,
        weight_map_type edge_weight,
        scene_type& scene,
        config_data_type cfg = config_data_type())
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_vertex_pos { vertex_pos }
    , m_edge_weight { edge_weight }
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

    auto graph() const -> const graph_type& { return m_g; }
    auto vertex_id() const -> vertex_id_type { return m_vertex_id; }
    auto vertex_position() const -> position_map_type { return m_vertex_pos; }
    auto edge_weight() const -> weight_map_type { return m_edge_weight; }

    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }
    auto default_data() const -> const config_data_type& { return m_defaults; }
    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }
    auto config_api() const -> const config_api_type& { return m_config_api; }
    auto config_api() -> config_api_type& { return m_config_api; }

    inline auto draw(const config_data_type& cfg) -> void
    {
        visit_vertices([this, &cfg](const auto& v) { m_impl.draw(v, cfg); });
        visit_edges([this, &cfg](const auto& e) { m_impl.draw(e, cfg); });
    }

    inline auto draw_layout() -> void
    {
        visit_vertices([this](const auto& v) { m_impl.draw_layout(v); });
        visit_edges([this](const auto& e) { m_impl.draw_layout(e); });
    }

    inline auto clear() -> void
    {
        visit_edges([this](const auto& e) { m_impl.clear(e); });
        visit_vertices([this](const auto& v) { m_impl.clear(v); });
    }

private:
    using graph_renderer_impl = detail::graph_renderer_impl;
    using vertex_properties = detail::vertex_properties;
    using edge_properties = detail::edge_properties;

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< vertex_id_type >::value_type,
                  vertex_properties::id_type >);

    static_assert(
        std::is_convertible_v<
            typename boost::property_traits< weight_map_type >::value_type,
            edge_properties::weight_type >);

    template < typename UnaryOperation >
    requires std::invocable< UnaryOperation, const vertex_properties& >
    auto visit_vertices(UnaryOperation f) const
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            f(make_vertex_properties(v));
    }

    template < typename UnaryOperation >
    requires std::invocable< UnaryOperation, const edge_properties& >
    auto visit_edges(UnaryOperation f) const
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            f(make_edge_properties(e));
    }

    inline auto make_vertex_properties(vertex_type v) const
    {
        auto&& id = boost::get(vertex_id(), v);

        const auto& [x, y, z] = boost::get(vertex_position(), v);
        auto&& pos = vertex_properties::position_type(x, y, z);

        return vertex_properties { .id = std::move(id), .pos = std::move(pos) };
    }

    inline auto make_edge_properties(edge_type e) const
    {
        const auto source = boost::source(e, graph());
        const auto target = boost::target(e, graph());
        auto weight = boost::get(edge_weight(), e);

        return edge_properties { .source = make_vertex_properties(source),
                                 .target = make_vertex_properties(target),
                                 .weight = weight };
    }

    const graph_type& m_g;
    vertex_id_type m_vertex_id;
    position_map_type m_vertex_pos;
    weight_map_type m_edge_weight;

    scene_type& m_scene;
    config_data_type m_config, m_defaults;
    config_api_type m_config_api;

    graph_renderer_impl m_impl;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
