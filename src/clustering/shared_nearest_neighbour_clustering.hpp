// Contains a generic Shared Nearest Neighbour Clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include "detail/hash.hpp"
#include "detail/shared_nearest_neighbour_clustering.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <cassert>

namespace clustering
{

// Generic SNN Clustering algorithm.
// NOTE: Will produce weird clusters if the num of vertices exceeds the cluster
// type max / 2
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
    using cluster_limits = std::numeric_limits< cluster_type >;

    assert(threshold >= 0 && "negative thresholds make no sense");

    // Early exit
    if (boost::graph::has_no_vertices(g))
        return;

    // Isolate edges
    for (cluster_type c = 0;
         auto v : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, v, c++);

    // Early exit.
    if (boost::graph::has_no_edges(g))
        return;

    // Fill edge proximity map.
    proximity_storage edge_proximity;
    detail::shared_nearest_neighbour(
        g, boost::make_assoc_property_map(edge_proximity));

    // Fill cluster map.
    for (cluster_type shared = cluster_limits::max();
         auto e : boost::make_iterator_range(boost::edges(g)))
    {
        const auto iter = edge_proximity.find(e);
        assert(iter != std::cend(edge_proximity));
        const auto proximity = iter->second;
        const bool share_t_neighbours = (proximity >= threshold);

        if (share_t_neighbours)
        {
            const auto c1 = boost::get(vertex_cluster, boost::source(e, g));
            const auto c2 = boost::get(vertex_cluster, boost::target(e, g));

            if (detail::is_shared(c1) or detail::is_shared(c2))
            {
                const auto c = detail::shared_between(c1, c2);
                boost::put(vertex_cluster, boost::source(e, g), c);
                boost::put(vertex_cluster, boost::target(e, g), c);
            }
            else
            {
                boost::put(vertex_cluster, boost::source(e, g), shared);
                boost::put(vertex_cluster, boost::target(e, g), shared--);
            }
        }
    }
}

} // namespace clustering

#endif // CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
