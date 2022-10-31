// Contains a generic k-Spanning Tree algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
#define CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP

#include "detail/hash.hpp"
#include "detail/k_spanning_tree_clustering.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <type_traits>
#include <unordered_set>

namespace clustering
{

// k-Spanning Tree graph clustering algorithm.
template <
    typename Graph,
    typename MinimumSpanningTreeFunc,
    typename WeightMap,
    typename ClusterMap >
auto k_spanning_tree_clustering(
    const Graph& g,
    int k,
    MinimumSpanningTreeFunc fill_mst,
    WeightMap edge_weight,
    ClusterMap vertex_cluster) -> void
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
    using edge_type = typename graph_traits::edge_descriptor;
    using edges_set = std::unordered_set< edge_type, detail::edge_hash >;
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;

    static_assert(std::is_invocable_v<
                  MinimumSpanningTreeFunc,
                  const Graph&,
                  std::insert_iterator< edges_set >,
                  WeightMap >);

    static_assert(std::is_integral_v< cluster_type >);

    assert(k >= 1 && "cannot form negative clusters");

    // Early exit.
    if (detail::has_no_vertices(g))
        return;

    // Fill mst edge set.
    auto mst = edges_set();
    fill_mst(g, std::inserter(mst, std::begin(mst)), edge_weight);

    // Remove k-1 highest weight edges from the mst set.
    const auto steps = k - 1;
    for (decltype(k) i = 0; i < steps; ++i)
    {
        const auto iter = detail::max_weighted(mst, edge_weight);
        const bool out_of_edges = (iter == std::cend(mst));

        if (out_of_edges)
            break;
        else
            mst.erase(iter);
    }

    // Assign vertices to clusters.
    constexpr cluster_type mst_cluster = 0;
    cluster_type isolated_cluster = mst_cluster + 1;

    for (auto v : boost::make_iterator_range(boost::vertices(g)))
    {
        assert(isolated_cluster != mst_cluster);

        if (detail::is_referenced(g, mst, v))
            boost::put(vertex_cluster, v, mst_cluster);
        else
            boost::put(vertex_cluster, v, isolated_cluster++);
    }

    assert(isolated_cluster >= static_cast< cluster_type >(k));
}

} // namespace clustering

#endif // CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
