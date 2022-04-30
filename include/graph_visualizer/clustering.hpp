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

namespace GV::Clustering {

// k-Spanning Tree clustering algorithm implementation, given a boost graph, the number of clusters,
// a Minimum Spanning Tree algorithm and an edge descriptor (less-than) Compare function
template <typename MSTAlgo, typename Graph,
          typename Compare = std::less<typename boost::graph_traits<Graph>::edge_descriptor>>
auto k_spanning_tree(const Graph& g, unsigned k) -> Graph
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;

    static_assert(std::is_invocable_r_v<Graph, MSTAlgo, Graph>, "expected MST algorithm");
    static_assert(std::is_invocable_r_v<bool, Compare, Edge, Edge>, "cannot compare edges");

    assert(k >= 1 && "cannot form negative clusters"); // k is actually the number of clusters

    auto mst = MSTAlgo{}(g);

    const auto iters = k - 1;
    for (decltype(k) i = 0; i < iters; ++i) {
        const auto& [first, last] = boost::edges(mst);
        const auto iter = std::min_element(first, last, Compare{});

        if (iter == last) break; // cannot extract more edges

        boost::remove_edge(*iter, mst);
    }

    assert(boost::num_edges(mst) <= boost::num_edges(g));
    assert(boost::num_vertices(mst) == boost::num_vertices(g));
    return mst;
}

// template <typename Graph, typename Done>
// void shared_nearest_neighbour(Graph& g, Done done) // TODO
// {}

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