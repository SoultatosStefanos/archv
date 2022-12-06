// Contains an implementation header for th snn clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include "misc/algorithm.hpp" // for set_intersection

#include <boost/graph/adjacency_list.hpp> // for source, target, etc

namespace clustering::detail
{

// Fills a property map of edge keys with the number of shared neighbours of
// each src and target.
template < typename Graph, typename ProximityMap >
auto shared_nearest_neighbour(const Graph& g, ProximityMap edge_proximity)
    -> void
{
    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using vertices_type = std::vector< vertex_type >;

    // for each edge (u, v), u -> v
    for (auto edge : boost::make_iterator_range(boost::edges(g)))
    {
        const auto u = boost::source(edge, g);
        const auto [ubegin, uend] = boost::adjacent_vertices(u, g);

        const auto v = boost::target(edge, g);
        const auto [vbegin, vend] = boost::adjacent_vertices(v, g);

        vertices_type intersection;
        misc::set_intersection(
            ubegin, uend, vbegin, vend, std::back_inserter(intersection));

        boost::put(edge_proximity, edge, intersection.size());
    }
}

// Little heuristic, not complete by any means.
// Should a graph naturally exceed this, no biggie.
template < typename Cluster >
constexpr auto shared_threshold()
{
    return std::numeric_limits< Cluster >::max() / 2;
}

template < typename Cluster >
constexpr auto is_shared(Cluster c) -> bool
{
    return c > shared_threshold< Cluster >();
}

template < typename Cluster >
constexpr auto shared_between(Cluster c1, Cluster c2)
{
    return std::max(c1, c2);
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
