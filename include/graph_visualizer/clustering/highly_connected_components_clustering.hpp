// Contains a generic Highly Connected Components clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "merge.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/isomorphism.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <utility>

namespace GV::Clustering {

// Generic Highly Connected Componentes Clustering algorithm
// O(MinimumCut + EdgeConnectivity + V + E)
template <typename MutableGraph, typename MinimumCut, typename EdgeConnectivity>
void highly_connected_components_clustering(MutableGraph& g, MinimumCut min_cut,
                                            EdgeConnectivity ec)
{
    using Partition = std::pair<MutableGraph, MutableGraph>;
    using EdgesNum = typename boost::graph_traits<MutableGraph>::edges_size_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );

    static_assert(std::is_trivially_copyable_v<MinimumCut>);
    static_assert(std::is_trivially_copyable_v<EdgeConnectivity>);
    static_assert(std::is_invocable_r_v<Partition, MinimumCut, MutableGraph>);
    static_assert(std::is_invocable_r_v<EdgesNum, EdgeConnectivity, MutableGraph, Partition>);

    auto partition = minimum_cut(g);

    assert(boost::isomorphism(g, merge(partition.first, partition.second)));

    if (edge_connectivity(g, partition) > (boost::num_vertices(g) / 2)) return; // break recursion

    highly_connected_components_clustering_impl(partition.first, minimum_cut, edge_connectivity);
    highly_connected_components_clustering_impl(partition.second, minimum_cut, edge_connectivity);

    g = merge(partition.first, partition.second);
}

} // namespace GV::Clustering