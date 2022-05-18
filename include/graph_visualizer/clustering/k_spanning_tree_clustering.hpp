// Contains a geneic k-Spanning Tree algorithm
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename MutableGraph, typename PredecessorMap>
inline void retain_mst(MutableGraph& g, PredecessorMap p_map)
{
    // Remove all edges (u, v) when p_map[u] != v and p_map[v] != u
    boost::remove_edge_if(
        [p_map, &g](auto edge) {
            return boost::get(p_map, boost::source(edge, g)) != boost::target(edge, g)
                   and boost::get(p_map, boost::target(edge, g)) != boost::source(edge, g);
        },
        g);
}

template <typename MutableGraph, typename MinimumSpanningTree, typename PredecessorMap,
          typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
void k_spanning_tree_clustering_impl(MutableGraph& g, unsigned k, MinimumSpanningTree mst,
                                     PredecessorMap p_map, WeightMap edge_weight)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;

    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<PredecessorMap, Vertex>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<WeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<MinimumSpanningTree>);
    static_assert(std::is_trivially_copyable_v<PredecessorMap>);
    static_assert(std::is_trivially_copyable_v<WeightMap>);
    static_assert(std::is_invocable_v<MinimumSpanningTree, MutableGraph, PredecessorMap>);

    assert(k >= 1 && "cannot form negative clusters");

    if (boost::num_edges(g) == 0) return; // early exit

    mst(g, p_map); // fill predecessor map

    retain_mst(g, p_map);

    const auto iters = k - 1;
    for (decltype(k) i = 0; i < iters; ++i) {
        const auto& [first, last] = boost::edges(g);

        const auto iter = std::max_element(first, last, [&](const auto& lhs, const auto& rhs) {
            return boost::get(edge_weight, lhs) < boost::get(edge_weight, rhs);
        });

        if (iter == last) return; // cannot extract more edges

        boost::remove_edge(*iter, g);
    }
}

} // namespace Details

// Generic k-Spanning Tree clustering algorithm
// O(MST + k * (E * V + E) / V )
template <typename MutableGraph, typename MinimumSpanningTree, typename PredecessorMap,
          typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
inline void k_spanning_tree_clustering(MutableGraph& g, unsigned k, WeightMap edge_weight,
                                       PredecessorMap p_map, MinimumSpanningTree mst)
{
    Details::k_spanning_tree_clustering_impl(g, k, mst, p_map, edge_weight);
}

// Generic k-Spanning Tree clustering algorithm, with default boost edge_weight property
// O(MST + k * (E * V + E) / V )
template <typename MutableGraph, typename MinimumSpanningTree, typename PredecessorMap>
inline void k_spanning_tree_clustering(MutableGraph& g, unsigned k, PredecessorMap p_map,
                                       MinimumSpanningTree mst)
{
    Details::k_spanning_tree_clustering_impl(g, k, mst, p_map, boost::get(boost::edge_weight, g));
}

// Generic k-Spanning Tree clustering algorithm, with default boost edge_weight and predecessor map
// represented as an std::vector (efficient for graphs with vecS internal storage)
// O(MST + k * (E * V + E) / V )
template <typename MutableGraph, typename MinimumSpanningTree>
inline void k_spanning_tree_clustering(MutableGraph& g, unsigned k, MinimumSpanningTree mst)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using PredecessorMap = std::vector<Vertex>;

    PredecessorMap p_map(boost::num_vertices(g));
    Details::k_spanning_tree_clustering_impl(g, k, mst, &p_map[0],
                                             boost::get(boost::edge_weight, g));
}

} // namespace GV::Clustering