// Contains a private module for the k spanning tree clustering header.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_K_SPANNING_TREE_CLUSTERING_HPP
#define CLUSTERING_DETAIL_K_SPANNING_TREE_CLUSTERING_HPP

#include <algorithm>                      // for max_element, etc
#include <boost/graph/adjacency_list.hpp> // for vertices, etc

namespace clustering::detail
{

// Returns the max weighted edge of an edge set.
template < typename EdgeSet, typename WeightMap >
inline auto max_weighted(const EdgeSet& edges, WeightMap edge_weight)
{
    return std::max_element(
        std::cbegin(edges),
        std::cend(edges),
        [edge_weight](auto lhs, auto rhs) {
            return boost::get(edge_weight, lhs) < boost::get(edge_weight, rhs);
        });
}

// Returns true if vertex is referenced by any edge of an edge set.
template < typename Graph, typename EdgeSet, typename Vertex >
inline auto is_referenced(const Graph& g, const EdgeSet& edges, Vertex v)
{
    return std::find_if(
               std::cbegin(edges),
               std::cend(edges),
               [v, &g](auto e)
               { return boost::source(e, g) == v or boost::target(e, g) == v; })
        != std::cend(edges);
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_K_SPANNING_TREE_CLUSTERING_HPP
