// Contains a class responsible for rendering any architecture graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include "cluster_color_pool.hpp"
#include "degrees_ranked_evaluator.hpp"
#include "detail/graph_renderer.hpp"
#include "graph_config.hpp"

#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <boost/graph/adjacency_list.hpp>
#include <concepts>
#include <string_view>

namespace rendering
{

/***********************************************************
 * Concepts                                                *
 ***********************************************************/

// clang-format off
template < typename Class >
concept degrees_evaluator 
= requires
{
    typename Class::degree_type;
    typename Class::particles_type;
} && requires(Class val, typename Class::degree_type degrees)
{
    { val.in_degree_particles(degrees) } 
        -> std::same_as< typename Class::particles_type >;
    { val.out_degree_particles(degrees) }  
        -> std::same_as< typename Class::particles_type >;
};
// clang-format on

// clang-format off
template < typename Class >
concept cluster_color_coder 
= requires
{
    typename Class::cluster;
    typename Class::rgba_type;
} && requires(Class val,
    typename Class::cluster c, 
    typename Class::rgba_type rgba)
{
    { std::invoke(val, c) } -> std::same_as< decltype(rgba) >;
};
// clang-format on

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
    degrees_evaluator DegreesEvaluator = degrees_ranked_evaluator,
    cluster_color_coder ClusterColorCoder = cluster_color_pool >
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

    using graph_traits = boost::graph_traits< Graph >;

public:
    using graph_type = Graph;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;
    using degree_type = typename graph_traits::degree_size_type;
    using vertex_id_type = VertexID;
    using dependency_map_type = DependencyMap;

    using scene_type = Ogre::SceneManager;
    using config_data_type = graph_config;
    using config_api_type = graph_config_api;

    using degrees_evaluator_type = DegreesEvaluator;
    using cluster_color_coder_type = ClusterColorCoder;

    static_assert(std::is_convertible_v<
                  typename degrees_evaluator_type::particles_type,
                  std::optional< detail::vertex_renderer::name_type > >);

    static_assert(std::is_convertible_v<
                  degree_type,
                  typename degrees_evaluator_type::degree_type >);

    // Renders the graph with its layout.
    template < typename PositionMap >
    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        PositionMap vertex_pos,
        dependency_map_type edge_dependency,
        scene_type& scene,
        config_data_type cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT,
        degrees_evaluator_type degrees_eval = degrees_evaluator_type(),
        cluster_color_coder_type color_coder = cluster_color_coder_type());

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = default;

    ~graph_renderer();

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto graph() const -> const auto& { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_dependency() const -> auto { return m_edge_dependency; }

    auto scene() const -> const scene_type& { return m_scene; }
    auto scene() -> scene_type& { return m_scene; }

    auto default_data() const -> const config_data_type& { return m_defaults; }
    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }
    auto config_api() const -> const config_api_type& { return m_cfg_api; }
    auto config_api() -> config_api_type& { return m_cfg_api; }

    auto resource_group() const -> auto* { return m_resource_group.data(); }

    auto get_degrees_evaluator() const -> const auto& { return m_degrees_eval; }
    auto get_degrees_evaluator() -> auto& { return m_degrees_eval; }

    auto get_cluster_color_coder() const -> const auto&
    {
        return m_cluster_col_coder;
    }
    auto get_cluster_color_coder() -> auto& { return m_cluster_col_coder; }

    template < typename PositionMap >
    auto render_layout(PositionMap vertex_pos) -> void;

    template < typename ScaleMap >
    auto render_scaling(ScaleMap vertex_scale) -> void;

    template < typename WeightMap >
    auto render_weights(WeightMap edge_weight) -> void;

    auto render_in_degree_particles() -> void;
    auto render_out_degree_particles() -> void;

    template < typename ClusterMap >
    auto render_clusters(ClusterMap vertex_cluster) -> void;

    auto hide_scaling() -> void;
    auto hide_weights() -> void;
    auto hide_clusters() -> void;

    auto draw(const config_data_type& cfg) -> void;
    auto draw(config_data_type&&) -> void = delete; // disallow temporaries

protected:
    template < typename UnaryOperation >
    auto visit_vertices(UnaryOperation f) const -> void;

    template < typename UnaryOperation >
    auto visit_edges(UnaryOperation f) const;

private:
    template < typename Tuple >
    static auto to_vector3(const Tuple& t);

    const graph_type& m_g;
    vertex_id_type m_vertex_id;
    dependency_map_type m_edge_dependency;

    scene_type& m_scene;
    config_data_type m_cfg, m_defaults;
    config_api_type m_cfg_api;
    std::string_view m_resource_group;

    detail::vertex_renderer m_vertex_renderer;
    detail::edge_renderer m_edge_renderer;

    degrees_evaluator_type m_degrees_eval;
    cluster_color_coder_type m_cluster_col_coder;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename PositionMap >
inline graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::
    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        PositionMap vertex_pos,
        dependency_map_type edge_dependency,
        scene_type& scene,
        config_data_type cfg,
        std::string_view resource_group,
        degrees_evaluator_type degrees_eval,
        cluster_color_coder_type color_coder)
: m_g { g }
, m_vertex_id { vertex_id }
, m_edge_dependency { edge_dependency }
, m_scene { scene }
, m_cfg { cfg }
, m_defaults { cfg }
, m_cfg_api { std::move(cfg) }
, m_resource_group { resource_group }
, m_vertex_renderer { scene, config_data(), resource_group }
, m_edge_renderer { scene, config_data(), resource_group }
, m_degrees_eval { std::move(degrees_eval) }
, m_cluster_col_coder { std::move(color_coder) }
{
    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept< PositionMap, vertex_type >));

    visit_vertices(
        [this, vertex_pos](auto v)
        {
            m_vertex_renderer.setup(
                boost::get(this->vertex_id(), v),
                to_vector3(boost::get(vertex_pos, v)));
        });

    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.setup(
                boost::get(this->vertex_id(), boost::source(e, graph())),
                boost::get(this->vertex_id(), boost::target(e, graph())),
                boost::get(this->edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::~graph_renderer()
{
    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.shutdown(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });

    visit_vertices([this](auto v)
                   { m_vertex_renderer.shutdown(boost::get(vertex_id(), v)); });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename UnaryOperation >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::visit_vertices(UnaryOperation f) const -> void
{
    static_assert(std::is_invocable_v< UnaryOperation, vertex_type >);
    for (auto v : boost::make_iterator_range(boost::vertices(graph())))
        f(v);
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename UnaryOperation >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::visit_edges(UnaryOperation f) const
{
    static_assert(std::is_invocable_v< UnaryOperation, edge_type >);
    for (auto e : boost::make_iterator_range(boost::edges(graph())))
        f(e);
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename PositionMap >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_layout(PositionMap vertex_pos) -> void
{
    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept< PositionMap, vertex_type >));

    visit_vertices(
        [this, vertex_pos](auto v)
        {
            m_vertex_renderer.render_position(
                boost::get(vertex_id(), v),
                to_vector3(boost::get(vertex_pos, v)));
        });

    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.render_position(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename ScaleMap >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_scaling(ScaleMap vertex_scale) -> void
{
    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept< ScaleMap, vertex_type >));

    visit_vertices(
        [this, vertex_scale](auto v)
        {
            m_vertex_renderer.render_scale(
                boost::get(vertex_id(), v),
                to_vector3(boost::get(vertex_scale, v)));
        });

    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.render_position(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename WeightMap >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_weights(WeightMap edge_weight) -> void
{
    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept< WeightMap, edge_type >));

    visit_edges(
        [this, edge_weight](auto e)
        {
            m_edge_renderer.render_weight(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e),
                boost::get(edge_weight, e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_in_degree_particles() -> void
{
    BOOST_CONCEPT_ASSERT((boost::BidirectionalGraphConcept< graph_type >));

    visit_vertices(
        [this](auto v)
        {
            m_vertex_renderer.render_in_degree_particles(
                boost::get(vertex_id(), v),
                get_degrees_evaluator().in_degree_particles(
                    boost::in_degree(v, graph())));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_out_degree_particles() -> void
{
    visit_vertices(
        [this](auto v)
        {
            m_vertex_renderer.render_out_degree_particles(
                boost::get(vertex_id(), v),
                get_degrees_evaluator().out_degree_particles(
                    boost::out_degree(v, graph())));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename ClusterMap >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::render_clusters(ClusterMap vertex_cluster) -> void
{
    // NOTE this assert somehow derped g++-11 :(
    // BOOST_CONCEPT_ASSERT(
    //     (boost::ReadablePropertyMapConcept< ClusterMap, vertex_type >));

    visit_vertices(
        [this, vertex_cluster](auto v)
        {
            m_vertex_renderer.render_cluster(
                boost::get(vertex_id(), v),
                get_cluster_color_coder()(boost::get(vertex_cluster, v)));
        });

    visit_edges(
        [this, vertex_cluster](auto e)
        {
            const auto src = boost::source(e, graph());
            const auto trgt = boost::target(e, graph());

            const auto src_c = boost::get(vertex_cluster, src);
            const auto trgt_c = boost::get(vertex_cluster, trgt);
            const bool within_same_cluster = (src_c == trgt_c);

            if (within_same_cluster)
                m_edge_renderer.render_cluster(
                    boost::get(vertex_id(), src),
                    boost::get(vertex_id(), trgt),
                    boost::get(edge_dependency(), e),
                    get_cluster_color_coder()(boost::get(vertex_cluster, src)));
            else // in case of rendered cluster
                m_edge_renderer.hide_cluster(
                    boost::get(vertex_id(), boost::source(e, graph())),
                    boost::get(vertex_id(), boost::target(e, graph())),
                    boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::hide_scaling() -> void
{
    visit_vertices(
        [this](auto v)
        { m_vertex_renderer.hide_scale(boost::get(vertex_id(), v)); });

    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.render_position(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::hide_weights() -> void
{
    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.hide_weight(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::hide_clusters() -> void
{
    visit_vertices(
        [this](auto v)
        { m_vertex_renderer.hide_cluster(boost::get(vertex_id(), v)); });

    visit_edges(
        [this](auto e)
        {
            m_edge_renderer.hide_cluster(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e));
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::draw(const config_data_type& cfg) -> void
{
    visit_vertices(
        [this, &cfg](auto v)
        { m_vertex_renderer.draw(boost::get(vertex_id(), v), cfg); });

    visit_edges(
        [this, &cfg](auto e)
        {
            m_edge_renderer.draw(
                boost::get(vertex_id(), boost::source(e, graph())),
                boost::get(vertex_id(), boost::target(e, graph())),
                boost::get(edge_dependency(), e),
                cfg);
        });
}

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
template < typename Tuple >
inline auto graph_renderer<
    Graph,
    VertexID,
    DependencyMap,
    DegreesEvaluator,
    ClusterColorCoder >::to_vector3(const Tuple& t)
{
    const auto& [x, y, z] = t;
    return Ogre::Vector3(x, y, z);
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template <
    typename Graph,
    typename VertexID,
    typename PositionMap,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
inline auto make_graph_renderer(
    const Graph& g,
    VertexID vertex_id,
    PositionMap vertex_pos,
    DependencyMap edge_dependency,
    Ogre::SceneManager& scene,
    graph_config cfg,
    std::string_view resource_group = Ogre::RGN_DEFAULT,
    DegreesEvaluator degrees_eval = DegreesEvaluator(),
    ClusterColorCoder cluster_col_coder = ClusterColorCoder())
{
    return graph_renderer<
        Graph,
        VertexID,
        DependencyMap,
        DegreesEvaluator,
        ClusterColorCoder >(
        g,
        vertex_id,
        vertex_pos,
        edge_dependency,
        scene,
        std::move(cfg),
        resource_group,
        std::move(degrees_eval),
        std::move(cluster_col_coder));
}

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
