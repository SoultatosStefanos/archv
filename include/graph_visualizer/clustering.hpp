// Contains multiple graph clustering algorithms.
// Namely k-Spanning Tree, Shared Nearest Neighbour, Betweeness Centrality Based, Highly Connected,
// Components, Maximal Clique Enumeration, Kernel k-means
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bc_clustering.hpp>
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <cassert>
#include <type_traits>
#include <unordered_map>

namespace GV::Clustering {

// TODO Optimize away copies, make automatic test cases

// k-Spanning Tree clustering algorithm implementation, given a boost graph, the number of clusters,
// a Minimum Spanning Tree algorithm and an edge descriptor (less-than) Compare function
template <typename Graph, typename MSTAlgo,
          typename Compare = std::less<typename boost::graph_traits<Graph>::edge_descriptor>>
auto k_spanning_tree(const Graph& g, unsigned k, MSTAlgo find_mst, Compare cmp = Compare{}) -> Graph
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;

    static_assert(std::is_trivially_copyable_v<MSTAlgo>);
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
// τ, a Shared Nearest Neighbour algorithm, and Hash/Pred functions
template <typename Graph, typename SNNAlgo,
          typename Hash = std::hash<typename boost::graph_traits<Graph>::edge_iterator>,
          typename Pred = std::equal_to<typename boost::graph_traits<Graph>::edge_iterator>>
auto shared_nearest_neighbour(Graph g, unsigned threshold, SNNAlgo find_snn) -> Graph
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using EdgeIterator = typename boost::graph_traits<Graph>::edge_iterator;
    using SNNWeight = decltype(threshold);
    using SNNMap = std::unordered_map<EdgeIterator, SNNWeight, Hash, Pred>;

    static_assert(std::is_trivially_copyable_v<SNNAlgo>);
    static_assert(std::is_trivially_copyable_v<EdgeIterator>);
    static_assert(std::is_trivially_copyable_v<Hash>);
    static_assert(std::is_trivially_copyable_v<Pred>);
    static_assert(std::is_invocable_r_v<SNNMap, SNNAlgo, Graph>, "expected SNN algorithm");

#ifndef NDEBUG
    const auto original = g; // cache original on debug builds
#endif

    auto snn_map = find_snn(g);
    assert(snn_map.size() == boost::num_edges(g));

    const auto& [first, last] = boost::edges(g);
    for (auto iter = first; iter != last; ++iter) {
        assert(snn_map.contains(iter));
        if (snn_map[iter] < threshold) boost::remove_edge(*iter, g);
    }

    assert(boost::num_edges(g) <= boost::num_edges(original));
    assert(boost::num_vertices(g) == boost::num_vertices(original));
    return g;
}

// template <typename Graph, typename Done>
// auto betweenness_centrality_based(const Graph& g, Done done)
// {}

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