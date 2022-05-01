// Contains multiple graph clustering algorithms.
// Namely k-Spanning Tree, Shared Nearest Neighbour, Betweeness Centrality Based, Highly Connected,
// Components, Maximal Clique Enumeration, Kernel k-means
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/bc_clustering.hpp>
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/property_map/property_map.hpp>
#include <cassert>
#include <type_traits>
#include <unordered_map>

namespace GV::Clustering {

// k-Spanning Tree clustering algorithm implementation, given a boost graph, the number of clusters,
// a Minimum Spanning Tree algorithm and an edge weight property map
template <typename MutableGraph, typename MSTAlgorithm, typename EdgeWeightMap>
void k_spanning_tree(MutableGraph& g, unsigned k, MSTAlgorithm mst, EdgeWeightMap edge_weight)
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<EdgeWeightMap>) );

    static_assert(std::is_trivially_copyable_v<MSTAlgorithm>);
    static_assert(std::is_trivially_copyable_v<EdgeWeightMap>);
    static_assert(std::is_invocable_v<MSTAlgorithm, MutableGraph>);

    assert(k >= 1 && "cannot form negative clusters");

    mst(g);

    const auto iters = k - 1;
    for (decltype(k) i = 0; i < iters; ++i) {
        const auto& [first, last] = boost::edges(g);
        const auto iter
            = std::max_element(first, last, [&edge_weight](const auto& lhs, const auto& rhs) {
                  return boost::get(edge_weight, lhs) < boost::get(edge_weight, rhs);
              });

        if (iter == last) return; // cannot extract more edges

        boost::remove_edge(*iter, g);
    }
}

// Shared Nearest Neighbour clustering algorithm implementation, given a boost graph, the threshold
// τ, a Shared Nearest Neighbour algorithm, and an edge-weight read write property map
template <typename MutableGraph, typename SNNAlgorithm, typename EdgeWeightMap>
void shared_nearest_neighbour(MutableGraph& g, SNNAlgorithm snn, EdgeWeightMap edge_weight,
                              typename boost::property_traits<EdgeWeightMap>::value_type threshold)
{
    using EdgeWeight = typename boost::property_traits<EdgeWeightMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<EdgeWeightMap>) );

    static_assert(std::is_trivially_copyable_v<SNNAlgorithm>);
    static_assert(std::is_trivially_copyable_v<EdgeWeight>);
    static_assert(std::is_trivially_copyable_v<EdgeWeightMap>);
    static_assert(std::is_invocable_v<SNNAlgorithm, MutableGraph, EdgeWeightMap>);

    snn(g, edge_weight);

    const auto& [first, last] = boost::edges(g);
    for (auto iter = first; iter != last; ++iter)
        if (boost::get(edge_weight, *iter) < threshold) boost::remove_edge(*iter, g);
}

// template <typename Graph, typename Done>
// void higly_connected_components(Graph& g, Done done) // TODO
// {}

// template <typename Graph, typename Done>
// void maximum_clique_enumeration(Graph& g, Done done) // TODO
// {}

// template <typename Graph, typename Done>
// void kernel_k_means(Graph& g, Done done) // TODO
// {}

} // namespace GV::Clustering