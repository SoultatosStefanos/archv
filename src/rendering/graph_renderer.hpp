// Contains a class responsible for rendering any architecture graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include "detail/edge_renderer.hpp"
#include "detail/vertex_renderer.hpp"

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

    Ogre::String edge_type_font_name;
    Ogre::Real edge_type_char_height;
    Ogre::ColourValue edge_type_color;
    Ogre::Real edge_type_space_width;

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
template < typename Graph, typename VertexID, typename DependencyMap >
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

    // Renders the graph with its layout.
    template < typename PositionMap >
    graph_renderer(
        const graph_type& g,
        vertex_id_type vertex_id,
        PositionMap vertex_pos,
        dependency_map_type edge_dependency,
        scene_type& scene,
        config_data_type cfg,
        std::string_view resource_group = Ogre::RGN_DEFAULT)
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

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = delete;

    ~graph_renderer()
    {
        visit_edges(
            [this](auto e)
            {
                m_edge_renderer.shutdown(
                    boost::get(vertex_id(), boost::source(e, graph())),
                    boost::get(vertex_id(), boost::target(e, graph())),
                    boost::get(edge_dependency(), e));
            });

        visit_vertices(
            [this](auto v)
            { m_vertex_renderer.shutdown(boost::get(vertex_id(), v)); });
    }

    auto operator=(const graph_renderer&) -> graph_renderer& = default;
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

    template < typename PositionMap >
    inline auto render_layout(PositionMap vertex_pos) -> void
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

    template < typename ScaleMap >
    inline auto render_scaling(ScaleMap vertex_scale) -> void
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

    template < typename WeightMap >
    inline auto render_weights(WeightMap edge_weight) -> void
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

    template < typename ParticleSystemFunc >
    requires std::invocable< ParticleSystemFunc, degree_type >
    inline auto render_in_degree_particle_systems(ParticleSystemFunc f) -> void
    {
        BOOST_CONCEPT_ASSERT((boost::BidirectionalGraphConcept< graph_type >));

        static_assert(std::is_same_v<
                      decltype(f(boost::in_degree(vertex_type(), graph()))),
                      std::optional< detail::vertex_renderer::name_type > >);

        visit_vertices(
            [this, particle_sytem = std::move(f)](auto v)
            {
                m_vertex_renderer.render_in_degree_particle_system(
                    boost::get(vertex_id(), v),
                    particle_sytem(boost::in_degree(v, graph())));
            });
    }

    template < typename ParticleSystemFunc >
    requires std::invocable< ParticleSystemFunc, degree_type >
    inline auto render_out_degree_particle_systems(ParticleSystemFunc f) -> void
    {
        static_assert(std::is_same_v<
                      decltype(f(boost::out_degree(vertex_type(), graph()))),
                      std::optional< detail::vertex_renderer::name_type > >);

        visit_vertices(
            [this, particle_sytem = std::move(f)](auto v)
            {
                m_vertex_renderer.render_out_degree_particle_system(
                    boost::get(vertex_id(), v),
                    particle_sytem(boost::out_degree(v, graph())));
            });
    }

    inline auto hide_scaling() -> void
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

    inline auto hide_weights() -> void
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

    inline auto draw(const config_data_type& cfg) -> void
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
    template < typename Tuple >
    inline static auto to_vector3(const Tuple& t)
    {
        const auto& [x, y, z] = t;
        return Ogre::Vector3(x, y, z);
    }

    const graph_type& m_g;
    vertex_id_type m_vertex_id;
    dependency_map_type m_edge_dependency;

    scene_type& m_scene;
    config_data_type m_cfg, m_defaults;
    config_api_type m_cfg_api;
    std::string_view m_resource_group;

    detail::vertex_renderer m_vertex_renderer;
    detail::edge_renderer m_edge_renderer;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
