// Contains the Louvain Method clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP
#define CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP

#include "detail/louvain_method_clustering.hpp"
#include "misc/random.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <concepts>

namespace clustering
{

// Generic Louvain Method clustering algorithm implementation.
//
// This algorithm is divided in 2 phases: Modularity Optimization and Community
// Aggregation. Just after the first is completed, the second takes place.
// Louvain will iteratively go through both until we get an optimized partition
// of the network. Modularity Optimization - At the beginning of this phase, the
// algorithm will randomly order all the nodes in the network such that, one by
// one, it will remove and insert it in a different community. This will
// continue until no significant variation in modularity is achieved (given by a
// constant defined below - min). Community Aggregation - After finalizing the
// first pass, every node belonging to the same community is merged into a
// single giant one and the links connecting these will be formed by the sum of
// the ones previously connecting nodes from the same different communities.
// From now on, there will also exist self-loops that represent the sum of all
// links in a given community (strictly connecting nodes inside of it) before
// being collapsed into a single one.
//
// See: https://en.wikipedia.org/wiki/Louvain_method
// NOTE: Currently only taking into account out edges.
template <
    typename Graph,
    typename WeightMap,
    typename ClusterMap,
    std::floating_point Modularity = float,
    typename UGenerator = std::mt19937 >
auto louvain_method_clustering(
    const Graph& g,
    WeightMap edge_weight,
    ClusterMap vertex_cluster,
    Modularity min = 0.1,
    UGenerator rng = misc::rng()) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using weight_map_traits = boost::property_traits< WeightMap >;
    using weight_type = typename weight_map_traits::value_type;
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;
    using community_type = cluster_type;

    using network = detail::
        network_properties< vertex_type, weight_type, community_type >;
    using dendrogram
        = detail::dendrogram< typename network::vertex_community_storage_type >;

    static_assert(std::is_arithmetic_v< vertex_type >);
    static_assert(std::is_arithmetic_v< community_type >);
    static_assert(std::is_arithmetic_v< weight_type >);

    // Early exit.
    if (boost::graph::has_no_vertices(g))
        return;

    // Early exit.
    if (boost::graph::has_no_edges(g))
    {
        detail::cluster_in_isolation(g, vertex_cluster);
        return;
    }

    dendrogram partitions;
    Modularity curr_mod { 0 };

    auto status = detail::network_status< network >(g, edge_weight);

    detail::modularity_optimization(g, status, edge_weight, min, rng);
    curr_mod = detail::modularity< Modularity >(status);

    auto&& lvl1_part = detail::renumber_communities(status.vertex_community);
    auto curr_sub = detail::community_aggregation(g, edge_weight, lvl1_part);
    partitions.push_back(std::move(lvl1_part));

    // Keeps partitioning until no significant modularity increase occurs.
    do
    {
        auto new_status = detail::network_status< network >(
            curr_sub.g, boost::make_assoc_property_map(curr_sub.edge_weight));

        detail::modularity_optimization(
            curr_sub.g,
            new_status,
            boost::make_assoc_property_map(curr_sub.edge_weight),
            min,
            rng);

        const auto new_mod = detail::modularity< Modularity >(new_status);

        if (new_mod - curr_mod < min)
            break;

        curr_mod = new_mod;

        auto&& part = detail::renumber_communities(new_status.vertex_community);

        curr_sub = detail::community_aggregation(
            curr_sub.g,
            boost::make_assoc_property_map(curr_sub.edge_weight),
            part);

        partitions.push_back(std::move(part));

    } while (true);

    detail::cluster_from_dendrogram(g, vertex_cluster, partitions);
}

} // namespace clustering

#endif // CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP
