// Contains a class responsible for rendering any boost::graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include "detail/graph_renderer.hpp"

#include <OGRE/OgreColourValue.h>
#include <OGRE/OgreSceneManager.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <cassert>
#include <string>
#include <type_traits>

namespace rendering
{

/***********************************************************
 * Graph renderer                                          *
 ***********************************************************/

// Will prepare a scene at a render window upon initialization.
template <
    typename Graph,
    typename VertexID,
    typename WeightMap,
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
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            PositionMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< VertexID >::value_type,
                  std::string >);

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< WeightMap >::value_type,
                  double >);

public:
    using graph_type = Graph;
    using vertex_id_map = VertexID;
    using weight_map = WeightMap;
    using position_map = PositionMap;
    using config_data_type = graph_config;
    using config_api_type = graph_config_api;

    graph_renderer(
        const graph_type& g,
        vertex_id_map vertex_id,
        weight_map edge_weight,
        position_map vertex_pos,
        Ogre::SceneManager* scene,
        config_data_type config = config_data_type())
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_edge_weight { edge_weight }
    , m_vertex_pos { vertex_pos }
    , m_scene { scene }
    , m_impl { m_scene, config }
    {
        assert(m_scene);
        setup();
    }

    ~graph_renderer() { shutdown(); }

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = default;

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto default_data() const -> const config_data_type&
    {
        return m_impl.default_data();
    }

    auto config_data() const -> const config_data_type&
    {
        return m_impl.config_data();
    }

    auto config_data() -> config_data_type& { return m_impl.config_data(); }

    auto config_api() const -> const config_api_type&
    {
        return m_impl.config_api();
    }

    auto config_api() -> config_api_type& { return m_impl.config_api(); }

    auto graph() const -> const auto& { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto vertex_pos() const -> auto { return m_vertex_pos; }
    auto scene() const -> auto* { return m_scene; }

    auto set_vertex_id(vertex_id_map vertex_id) -> void
    {
        shutdown();
        m_vertex_id = vertex_id;
        setup();
    }

    auto set_edge_weight(weight_map edge_weight) -> void
    {
        m_edge_weight = edge_weight;
    }

    auto set_vertex_pos(position_map vertex_pos) -> void
    {
        m_vertex_pos = vertex_pos;
        update();
    }

    auto set_scene(Ogre::SceneManager* scene) -> void
    {
        assert(scene);
        shutdown();
        m_scene = scene;
        setup();
    }

    auto draw(const config_data_type& cfg) const -> void
    {
        draw_vertices(cfg);
        draw_edges(cfg);
    }

private:
    using graph_type_traits = boost::graph_traits< graph_type >;
    using vertex_type = typename graph_type_traits::vertex_descriptor;
    using edge_type = typename graph_type_traits::edge_descriptor;

    using vertex_properties = detail::vertex_rendering_properties;
    using edge_properties = detail::edge_rendering_properties;

    auto make_vertex_properties(vertex_type v) const -> vertex_properties
    {
        const auto& id = boost::get(vertex_id(), v);
        const auto& pos = boost::get(vertex_pos(), v);
        return { id, Ogre::Vector3(pos.x, pos.y, pos.z) };
    }

    auto make_edge_properties(edge_type e) const -> edge_properties
    {
        const auto source = boost::source(e, graph());
        const auto target = boost::target(e, graph());
        return { make_vertex_properties(source),
                 make_vertex_properties(target) };
    }

    auto setup() const -> void
    {
        setup_vertices();
        setup_edges();
    }

    auto setup_vertices() const -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            m_impl.setup_vertex(make_vertex_properties(v));
    }

    auto setup_edges() const -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            m_impl.setup_edge(make_edge_properties(e));
    }

    auto update() const -> void
    {
        update_vertices();
        update_edges();
    }

    auto update_vertices() const -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            m_impl.update_vertex(make_vertex_properties(v));
    }

    auto update_edges() const -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            m_impl.update_edge(make_edge_properties(e));
    }

    auto draw_vertices(const config_data_type& cfg) const -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            m_impl.draw_vertex(make_vertex_properties(v), cfg);
    }

    auto draw_edges(const config_data_type& cfg) const -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            m_impl.draw_edge(make_edge_properties(e), cfg);
    }

    auto shutdown() const -> void
    {
        shutdown_vertices();
        shutdown_edges();
    }

    auto shutdown_vertices() const -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
            m_impl.shutdown_vertex(make_vertex_properties(v));
    }

    auto shutdown_edges() const -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
            m_impl.shutdown_edge(make_edge_properties(e));
    }

    const graph_type& m_g;
    vertex_id_map m_vertex_id;
    weight_map m_edge_weight;
    position_map m_vertex_pos;
    Ogre::SceneManager* m_scene { nullptr };
    detail::graph_renderer_impl m_impl;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
