// Contains an implementation header for th snn clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include "misc/algorithm.hpp"

#include <boost/graph/adjacency_list.hpp>

namespace clustering::detail
{

template < typename Graph >
inline auto has_no_edges(const Graph& g) -> bool
{
    return boost::num_edges(g) == 0;
}

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

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
