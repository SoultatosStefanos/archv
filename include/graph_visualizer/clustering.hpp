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
#include <concepts>
#include <type_traits>
#include <unordered_map>

namespace GV::Clustering {

namespace Details {

template <typename MutableGraph, typename MSTAlgorithm, typename EdgeWeightMap>
requires std::equality_comparable<typename boost::property_traits<EdgeWeightMap>::value_type>
void k_spanning_tree_impl(MutableGraph& g, unsigned k, MSTAlgorithm mst, EdgeWeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<EdgeWeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<MSTAlgorithm>);
    static_assert(std::is_trivially_copyable_v<EdgeWeightMap>);
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

// k-Spanning Tree clustering algorithm implementation, given a boost graph, the number of clusters,
// a Minimum Spanning Tree algorithm, and an edge-weight read write property map
template <typename MutableGraph, typename MSTAlgorithm, typename EdgeWeightMap>
requires std::equality_comparable<typename boost::property_traits<EdgeWeightMap>::value_type>
inline void k_spanning_tree(MutableGraph& g, unsigned k, MSTAlgorithm mst,
                            EdgeWeightMap edge_weight)
{
    Details::k_spanning_tree_impl(g, k, mst, edge_weight);
}

// k-Spanning Tree clustering algorithm implementation, given a boost graph, the number of clusters,
// and a Minimum Spanning Tree algorithm
template <typename MutableGraph, typename MSTAlgorithm>
inline void k_spanning_tree(MutableGraph& g, unsigned k, MSTAlgorithm mst)
{
    Details::k_spanning_tree_impl(g, k, mst, boost::get(boost::edge_weight, g));
}

namespace Details {

template <typename MutableGraph, typename SNNAlgorithm, typename EdgeWeightMap>
requires std::totally_ordered<typename boost::property_traits<EdgeWeightMap>::value_type>
void shared_nearest_neighbour_impl(
    MutableGraph& g, SNNAlgorithm snn,
    typename boost::property_traits<EdgeWeightMap>::value_type threshold, EdgeWeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<EdgeWeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<SNNAlgorithm>);
    static_assert(std::is_trivially_copyable_v<EdgeWeightMap>);
    static_assert(std::is_invocable_v<SNNAlgorithm, MutableGraph, EdgeWeightMap>);

    snn(g, edge_weight); // fill edge weights map

    do {
        const auto& [first, last] = boost::edges(g);

        const auto iter = std::find_if(first, last, [edge_weight, threshold](auto edge) {
            return boost::get(edge_weight, edge) < threshold;
        });

        if (iter == last) break; // did not find any edge weights bellow the threshold

        boost::remove_edge(*iter, g);
    }
    while (true);
}

} // namespace Details

// Shared Nearest Neighbour clustering algorithm implementation, given a boost graph, the threshold
// τ, a Shared Nearest Neighbour algorithm, and an edge-weight read write property map
template <typename MutableGraph, typename SNNAlgorithm, typename EdgeWeightMap>
requires std::totally_ordered<typename boost::property_traits<EdgeWeightMap>::value_type>
inline void
shared_nearest_neighbour(MutableGraph& g, SNNAlgorithm snn,
                         typename boost::property_traits<EdgeWeightMap>::value_type threshold,
                         EdgeWeightMap edge_weight)
{
    Details::shared_nearest_neighbour_impl(g, snn, threshold, edge_weight);
}

// Shared Nearest Neighbour clustering algorithm implementation, given a boost graph, the threshold
// τ, and a Shared Nearest Neighbour algorithm
template <typename MutableGraph, typename SNNAlgorithm, typename EdgeWeightMap>
requires std::totally_ordered<typename boost::property_traits<EdgeWeightMap>::value_type>
inline void
shared_nearest_neighbour(MutableGraph& g, SNNAlgorithm snn,
                         typename boost::property_traits<EdgeWeightMap>::value_type threshold)
{
    Details::shared_nearest_neighbour_impl(g, snn, threshold, boost::get(boost::edge_weight, g));
}

// template <typename MutableGraph>
// void higly_connected_components(MutableGraph& g) // TODO
// {
//     BOOST_CONCEPT_ASSERT((VertexListGraphConcept<MutableGraph>) );
//     BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<MutableGraph>) );
// }

// template <typename Graph, typename Done>
// void maximum_clique_enumeration(Graph& g, Done done) // TODO
// {}

// template <typename Graph, typename Done>
// void kernel_k_means(Graph& g, Done done) // TODO
// {}

} // namespace GV::Clustering