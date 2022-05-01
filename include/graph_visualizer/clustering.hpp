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
// a Minimum Spanning Tree algorithm and an edge descriptor (less-than) Compare function
template <typename Graph, typename MSTAlgo,
          typename Compare = std::less<typename boost::graph_traits<Graph>::edge_descriptor>>
auto k_spanning_tree(const Graph& g, unsigned k, MSTAlgo find_mst, Compare cmp = Compare{}) -> Graph
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;

    static_assert(std::is_trivially_copyable_v<MSTAlgo>);
    static_assert(std::is_trivially_copyable_v<Compare>);
    static_assert(std::is_invocable_r_v<Graph, MSTAlgo, Graph>, "expected MST algorithm");
    static_assert(std::is_invocable_r_v<bool, Compare, Edge, Edge>, "cannot compare edges");

    assert(k >= 1 && "cannot form negative clusters"); // k is actually the number of clusters

    auto mst = find_mst(g);

    const auto iters = k - 1;
    for (decltype(k) i = 0; i < iters; ++i) {
        const auto& [first, last] = boost::edges(mst);
        const auto iter = std::min_element(first, last, cmp);

        if (iter == last) break; // cannot extract more edges

        boost::remove_edge(*iter, mst);
    }

    assert(boost::num_edges(mst) <= boost::num_edges(g));
    assert(boost::num_vertices(mst) == boost::num_vertices(g));
    return mst;
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