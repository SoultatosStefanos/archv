// Contains the Louvain Method clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP
#define CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP

#include "detail/louvain_method_clustering.hpp"

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <concepts>

namespace clustering
{

// Generic Louvain Method clustering algorithm implementation.
// See: https://en.wikipedia.org/wiki/Louvain_method
// Uses the .js implementation from:
// https://github.com/upphiminn/jLouvain/blob/master/src/jLouvain.js
// NOTE: Currently only taking into account out edges.
// Must we dispatch on bidirectional graphs?
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
    Modularity min = 0.0,
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
    network status;
    Modularity q { 0 };

    // Returns the current partition.
    const auto partition = [&]() -> const auto&
    {
        assert(!partitions.empty());
        return partitions.back();
    };

    detail::update_network_status(g, status, edge_weight);

    detail::modularity_optimization(g, status, edge_weight, min, rng);
    q = detail::modularity< Modularity >(status);

    auto&& lvl_one_part = detail::renumber_communities(status.vertex_community);
    partitions.push_back(std::move(lvl_one_part));

    auto induced = detail::community_aggregation(g, edge_weight, partition());

    // Keeps partitioning the graph until no significant modularity increase
    // occurs.
    do
    {
        detail::update_network_status(
            induced.g,
            status,
            boost::make_assoc_property_map(induced.edge_weight));

        detail::modularity_optimization(
            induced.g,
            status,
            boost::make_assoc_property_map(induced.edge_weight),
            min,
            rng);

        const auto new_q = detail::modularity< Modularity >(status);
        const auto delta_q = new_q - q;

        if (delta_q < min)
            break;

        q = new_q;

        auto&& new_part = detail::renumber_communities(status.vertex_community);
        partitions.push_back(std::move(new_part));

        induced = detail::community_aggregation(
            g,
            boost::make_assoc_property_map(induced.edge_weight),
            partition());

    } while (true);

    detail::cluster_from_dendrogram(vertex_cluster, partitions);
}

} // namespace clustering

#endif // CLUSTERING_LOUVAIN_METHOD_CLUSTERING_HPP
