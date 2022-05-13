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

template <typename MutableGraph, typename MSTAlgorithm, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
void k_spanning_tree_clustering_impl(MutableGraph& g, unsigned k, MSTAlgorithm mst,
                                     WeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<WeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<MSTAlgorithm>);
    static_assert(std::is_trivially_copyable_v<WeightMap>);
    static_assert(std::is_invocable_r_v<MutableGraph, MSTAlgorithm, MutableGraph>);

    assert(k >= 1 && "cannot form negative clusters");

    g = mst(g);

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
template <typename MutableGraph, typename MSTAlgorithm, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
inline void k_spanning_tree_clustering(MutableGraph& g, unsigned k, WeightMap edge_weight,
                                       MSTAlgorithm mst)
{
    Details::k_spanning_tree_clustering_impl(g, k, mst, edge_weight);
}

// Generic k-Spanning Tree clustering algorithm, with default boost edge_weight property
template <typename MutableGraph, typename MSTAlgorithm>
inline void k_spanning_tree_clustering(MutableGraph& g, unsigned k, MSTAlgorithm mst)
{
    Details::k_spanning_tree_clustering_impl(g, k, mst, boost::get(boost::edge_weight, g));
}

} // namespace GV::Clustering