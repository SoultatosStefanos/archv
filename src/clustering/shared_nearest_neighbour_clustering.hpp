// Contains a generic Shared Nearest Neighbour Clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include "detail/hash.hpp"
#include "detail/shared_nearest_neighbour_clustering.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <cassert>

namespace clustering
{

// Generic SNN Clustering algorithm.
template < typename Graph, typename ClusterMap >
auto shared_nearest_neighbour_clustering(
    const Graph& g, int threshold, ClusterMap vertex_cluster) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;
    using edge_type = typename graph_traits::edge_descriptor;
    using proximity_type = decltype(threshold);
    using proximity_storage
        = std::unordered_map< edge_type, proximity_type, detail::edge_hash >;
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;

    // Isolate edges
    for (cluster_type c = 0;
         auto v : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, v, c++);

    // Early exit.
    if (detail::has_no_edges(g))
        return;

    // Fill edge proximity map.
    proximity_storage edge_proximity;
    detail::shared_nearest_neighbour(
        g, boost::make_assoc_property_map(edge_proximity));

    // Fill cluster map.
    constexpr cluster_type shared = std::numeric_limits< cluster_type >::max();
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        const auto iter = edge_proximity.find(e);
        assert(iter != std::cend(edge_proximity));
        const auto proximity = iter->second;
        const bool share_t_neighbours = (proximity >= threshold);

        if (share_t_neighbours)
        {
            boost::put(vertex_cluster, boost::source(e, g), shared);
            boost::put(vertex_cluster, boost::target(e, g), shared);
        }
    }
}

} // namespace clustering

#endif // CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
