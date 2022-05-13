// Contains a generic Highly Connected Components clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <utility>

namespace GV::Clustering {

namespace Details {

template <typename MutableGraph, typename MinimumCut, typename EdgeConnectivity, typename Merge>
void highly_connected_components_clustering_impl(MutableGraph& g, MinimumCut min_cut,
                                                 EdgeConnectivity ec, Merge merge)
{
    using Partition = std::pair<MutableGraph, MutableGraph>;
    using EdgesNum = typename boost::graph_traits<MutableGraph>::edges_size_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );

    static_assert(std::is_trivially_copyable_v<MinimumCut>);
    static_assert(std::is_trivially_copyable_v<EdgeConnectivity>);
    static_assert(std::is_trivially_copyable_v<Merge>);
    static_assert(std::is_invocable_r_v<Partition, MinimumCut, MutableGraph>);
    static_assert(std::is_invocable_r_v<EdgesNum, EdgeConnectivity, MutableGraph, Partition>);
    static_assert(std::is_invocable_r_v<MutableGraph, Merge, MutableGraph, MutableGraph>);

    auto partition = min_cut(g);

    if (ec(g, partition) > (boost::num_vertices(g) / 2)) return; // break recursion

    highly_connected_components_clustering_impl(partition.first, min_cut, ec);
    highly_connected_components_clustering_impl(partition.second, min_cut, ec);

    g = merge(partition.first, partition.second);
}

} // namespace Details

template <typename MutableGraph, typename MinimumCut, typename EdgeConnectivity, typename Merge>
inline void highly_connected_components_clustering(MutableGraph& g, MinimumCut min_cut,
                                                   EdgeConnectivity ec, Merge merge)
{
    Details::highly_connected_components_clustering_impl(g, min_cut, ec, merge);
}

// TODO More

} // namespace GV::Clustering