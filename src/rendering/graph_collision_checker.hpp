// Contains a class responsible for checking for collisions with graph
// renderable entities.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_COLLISION_CHECKER_HPP
#define RENDERING_GRAPH_COLLISION_CHECKER_HPP

#include <OGRE/OgreRay.h>
#include <OGRE/OgreSceneManager.h>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <optional>

namespace rendering
{

/***********************************************************
 * Graph Collision Checker                                 *
 ***********************************************************/

// Generic graph collision checker, concering the graph's renderable properties.
// NOTE: Expects the graph to have already been rendered at the passed scene.
template < typename Graph, typename IDMap, typename DependencyMap >
class graph_collision_checker
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            IDMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            DependencyMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;
    using id_traits = boost::property_traits< IDMap >;
    using dependency_traits = boost::property_traits< DependencyMap >;

public:
    using graph_type = Graph;
    using id_map_type = IDMap;
    using id_type = typename id_traits::value_type;
    using dependency_map_type = DependencyMap;
    using dependency_type = typename dependency_traits::value_type;

    using ray_type = Ogre::Ray;
    using scene_type = Ogre::SceneManager;

    using collision_query = std::optional< id_type >;

    graph_collision_checker(
        const graph_type& g,
        id_map_type vertex_id,
        dependency_map_type edge_dependency,
        const scene_type& scene);

    auto graph() const -> const auto& { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_dependency() const -> auto { return m_edge_dependency; }
    auto scene() const -> const auto& { return m_scene; }

    auto vertex_collision(const ray_type& ray) const -> collision_query;

private:
    auto vertex_collision(const ray_type& ray, const id_type& id) const -> bool;

    const graph_type& m_g;
    id_map_type m_vertex_id;
    dependency_map_type m_edge_dependency;
    const scene_type& m_scene;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename IDMap, typename DependencyMap >
inline graph_collision_checker< Graph, IDMap, DependencyMap >::
    graph_collision_checker(
        const graph_type& g,
        id_map_type vertex_id,
        dependency_map_type edge_dependency,
        const scene_type& scene)
: m_g { g }
, m_vertex_id { vertex_id }
, m_edge_dependency { edge_dependency }
, m_scene { scene }
{
}

template < typename Graph, typename IDMap, typename DependencyMap >
inline auto
graph_collision_checker< Graph, IDMap, DependencyMap >::vertex_collision(
    const ray_type& ray) const -> collision_query
{
    using boost::get;
    using boost::make_iterator_range;
    using boost::vertices;

    for (auto v : make_iterator_range(vertices(graph())))
        if (vertex_collision(ray, get(vertex_id(), v)))
            return get(vertex_id(), v);

    return std::nullopt;
}

template < typename Graph, typename IDMap, typename DependencyMap >
inline auto
graph_collision_checker< Graph, IDMap, DependencyMap >::vertex_collision(
    const ray_type& ray, const id_type& id) const -> bool
{
    static_assert(std::is_convertible_v< id_type, Ogre::String >);
    assert(scene().hasEntity(id));
    auto* e = m_scene.getEntity(id);
    assert(e);
    return ray.intersects(e->getWorldBoundingBox()).first;
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template < typename Graph, typename IDMap, typename DependencyMap >
inline auto make_graph_collision_checker(
    const Graph& g,
    IDMap vertex_id,
    DependencyMap edge_dependency,
    const Ogre::SceneManager& scene)
{
    return graph_collision_checker< Graph, IDMap, DependencyMap >(
        g, vertex_id, edge_dependency, scene);
}

} // namespace rendering

#endif // RENDERING_GRAPH_COLLISION_CHECKER_HPP
