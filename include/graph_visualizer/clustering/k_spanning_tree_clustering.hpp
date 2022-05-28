// Contains a geneic k-Spanning Tree algorithm
// Soultatos Stefanos 2022

#pragma once

#include "filtered_graph.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

struct EdgeHash {
    template <typename Edge>
    auto operator()(Edge e) const -> std::size_t
    {
        return std::hash<uint64_t>{}(reinterpret_cast<uint64_t>(e.get_property()));
    }
};

template <typename Graph, typename MinimumSpanningTree, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type> and std::
    equality_comparable<typename boost::graph_traits<Graph>::edge_descriptor>
auto k_spanning_tree_clustering_impl(const Graph& g, unsigned k,
                                     MinimumSpanningTree minimum_spanning_tree,
                                     WeightMap edge_weight) -> Filtered<Graph>
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((
        boost::ReadWritePropertyMapConcept<WeightMap,
                                           typename boost::graph_traits<Graph>::edge_descriptor>) );

    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using MSTEdges = std::unordered_set<Edge, EdgeHash>;

    static_assert(std::is_invocable_v<MinimumSpanningTree, Graph, std::insert_iterator<MSTEdges>>);

    assert(k >= 1 && "cannot form negative clusters");

    if (boost::num_edges(g) == 0) return make_filtered(g); // early exit

    MSTEdges mst_edges;
    minimum_spanning_tree(g, std::inserter(mst_edges, std::begin(mst_edges)));

    for (decltype(k) i = 0, iters = k - 1; i < iters; ++i) { // remove max edge from mst k-1 times
        const auto iter = std::max_element(
            std::begin(mst_edges), std::end(mst_edges), [edge_weight](auto lhs, auto rhs) {
                return boost::get(edge_weight, lhs) < boost::get(edge_weight, rhs);
            });

        if (iter == std::end(mst_edges)) break; // cannot extract more edges

        mst_edges.erase(iter);
    }

    return make_filtered(g, [mst = std::move(mst_edges)](auto edge) { return mst.contains(edge); });
}

} // namespace Details

// Generic k-Spanning Tree clustering algorithm
// O(MST + k + V + E)
template <typename Graph, typename MinimumSpanningTree, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type> and std::
    equality_comparable<typename boost::graph_traits<Graph>::edge_descriptor>
inline auto k_spanning_tree_clustering(Graph& g, unsigned k, WeightMap edge_weight,
                                       MinimumSpanningTree minimum_spanning_tree)
{
    return Details::k_spanning_tree_clustering_impl(g, k, minimum_spanning_tree, edge_weight);
}

// Generic k-Spanning Tree clustering algorithm, with default boost edge_weight property
// O(MST + k + V + E)
template <typename Graph, typename MinimumSpanningTree>
requires std::equality_comparable<typename boost::graph_traits<Graph>::edge_descriptor>
inline auto k_spanning_tree_clustering(Graph& g, unsigned k,
                                       MinimumSpanningTree minimum_spanning_tree)
{
    return Details::k_spanning_tree_clustering_impl(g, k, minimum_spanning_tree,
                                                    boost::get(boost::edge_weight, g));
}

} // namespace GV::Clustering