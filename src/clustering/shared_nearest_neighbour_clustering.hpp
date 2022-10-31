// Contains a generic Shared Nearest Neighbour Clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include "detail/shared_nearest_neighbour_clustering.hpp"

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace clustering
{

template < typename Graph, typename ClusterMap, typename ProximityMap >
auto shared_nearest_neighbour_clustering(
    const Graph& g,
    ClusterMap vertex_cluster,
    typename boost::property_traits< ProximityMap >::value_type threshold,
    ProximityMap edge_proximity) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ProximityMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

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
    detail::shared_nearest_neighbour(g, edge_proximity);

    // Fill cluster map.
    constexpr cluster_type shared = std::numeric_limits< cluster_type >::max();
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        const auto proximity = boost::get(edge_proximity, e);
        const bool share_t_neighbours = (proximity >= threshold);

        if (share_t_neighbours)
        {
            boost::put(vertex_cluster, boost::source(e, g), shared);
            boost::put(vertex_cluster, boost::target(e, g), shared);
        }
    }
}

// Overload for space optimization purposes.
template < typename Graph, typename ClusterMap >
inline auto shared_nearest_neighbour_clustering(
    const Graph& g, ClusterMap vertex_cluster, std::size_t threshold) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using proximity = std::size_t;
    using graph_traits = boost::graph_traits< Graph >;
    using edge_type = typename graph_traits::edge_descriptor;
    using proximity_storage = std::unordered_map< edge_type, proximity >;

    proximity_storage storage;
    shared_nearest_neighbour_clustering(
        g, vertex_cluster, threshold, boost::make_assoc_property_map(storage));
}

} // namespace clustering

#endif // CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
