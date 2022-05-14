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
#include <set>
#include <type_traits>
#include <utility>

namespace GV::Clustering {

namespace Details {

template <typename Graph, typename ParityMap>
requires std::totally_ordered<typename boost::graph_traits<Graph>::edge_descriptor>
auto min_cut_edge_set(const Graph& g, ParityMap parity)
{
    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using MinCutEdgeSet = std::set<Edge>;

    MinCutEdgeSet edges;

    const auto& [begin, end] = boost::edges(g);
    for (auto iter = begin; iter != end; ++iter) {
        const auto src = boost::source(*iter, g);
        const auto trgt = boost::target(*iter, g);

        if (boost::get(parity, src) != boost::get(parity, trgt)) // different parity
            edges.insert(boost::edge(src, trgt));
    }

    return edges;
}

template <typename Graph, typename ParityMap>
auto divide(const Graph& g, ParityMap parity)
{
    using Partition = std::pair<Graph, Graph>;
}

} // namespace Details

// Generic Highly Connected Componentes Clustering algorithm
template <typename MutableGraph, typename MinimumCut, typename ParityMap>
requires std::totally_ordered<typename boost::graph_traits<MutableGraph>::edge_descriptor>
void highly_connected_components_clustering(MutableGraph& g, MinimumCut min_cut,
                                            ParityMap parity = boost::dummy_property_map)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using ParityKey = typename boost::property_traits<ParityMap>::key_type;
    using ParityValue = typename boost::property_traits<ParityMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<ParityMap, Vertex>) );

    static_assert(std::is_trivially_copyable_v<MinimumCut>);
    static_assert(std::is_trivially_copyable_v<ParityMap>);
    static_assert(std::is_same_v<ParityKey, Vertex>);
    static_assert(std::is_same_v<ParityValue, bool>);
    static_assert(std::is_invocable_v<MinimumCut, MutableGraph, ParityMap>);

    min_cut(g, parity); // fill parity map

    const auto min_cut_edges = Details::min_cut_edge_set(g, parity);

    const auto edge_connectivity = min_cut_edges.size(); // EC = | MinCutEdgeSet |
    if (edge_connectivity > (boost::num_vertices(g) / 2)) return; // break recursion

    auto partition = Details::divide(g, parity);

    assert(std::none_of(std::begin(min_cut_edges), std::end(min_cut_edges),
                        [&g1 = partition.first, &g2 = partition.second](auto edge) {
                            const auto contains_cut_edge = [edge](const auto& g) {
                                const auto& [first, last] = boost::edges(g);

                                return std::find(first, last, edge) != last;
                            };

                            return contains_cut_edge(g1) or contains_cut_edge(g2);
                        })
           && "found cut edge on disconnected partitions");

    assert(!boost::isomorphism(g, merge(partition.first, partition.second)) && "invalid cut");

    highly_connected_components_clustering_impl(partition.first, min_cut, parity, cmp);
    highly_connected_components_clustering_impl(partition.second, min_cut, parity, cmp);

    g = merge(partition.first, partition.second); // merge partitions into a single clustered graph
}

} // namespace GV::Clustering