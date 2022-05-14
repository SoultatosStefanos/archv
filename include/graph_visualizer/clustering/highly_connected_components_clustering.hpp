// Contains a generic Highly Connected Components clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "merge.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/isomorphism.hpp>
#include <cassert>
#include <concepts>
#include <functional>
#include <set>
#include <type_traits>
#include <utility>

namespace GV::Clustering {

namespace Details {

// Returns the minimum set of edges needed to disconnect the map
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

        if (boost::get(parity, src) != boost::get(parity, trgt)) { // different parity
            const auto [edge, res] = boost::edge(src, trgt, g);
            assert(res && "could not get edge");

            edges.insert(edge);
        }
    }

    return edges;
}

template <typename MinCutEdgeSet>
inline auto edge_connectivity(const MinCutEdgeSet& min_cut_edges)
{
    return min_cut_edges.size(); // EC = | MinCutEdgeSet |
}

// Divide graph into two disconnected subgraphs, using parity map
template <typename Graph, typename ParityMap, typename MinCutEdgeSet>
auto divide(const Graph& g, ParityMap parity, const MinCutEdgeSet& min_cut_edges)
{
    using Partition = std::pair<Graph, Graph>;
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using Filtered
        = boost::filtered_graph<Graph, std::function<bool(Edge)>, std::function<bool(Vertex)>>;

    const auto keep_uncut_edges
        = [&min_cut_edges](auto edge) { return !min_cut_edges.contains(edge); };

    Filtered first_view{g, keep_uncut_edges, // filter for vertices in 'true' parity
                        [parity](auto vertex) { return boost::get(parity, vertex) == true; }};

    Filtered second_view{g, keep_uncut_edges, // filter for vertices in 'false' parity
                         [parity](auto vertex) { return boost::get(parity, vertex) == false; }};

    Graph first, second;
    boost::copy_graph(first_view, first);
    boost::copy_graph(second_view, second);

    assert(boost::num_vertices(g) == boost::num_vertices(first) + boost::num_vertices(second));
    return Partition{std::move(first), std::move(second)};
}

} // namespace Details

// Generic Highly Connected Componentes Clustering algorithm
template <typename MutableGraph, typename MinimumCut, typename ParityMap>
requires std::totally_ordered<typename boost::graph_traits<MutableGraph>::edge_descriptor>
void highly_connected_components_clustering(MutableGraph& g, ParityMap parity, MinimumCut min_cut)
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

    // recursion break conditions
    if (Details::edge_connectivity(min_cut_edges) == 0) return;
    if (Details::edge_connectivity(min_cut_edges) > boost::num_vertices(g) / 2) return;

    auto partition = Details::divide(g, parity, min_cut_edges);

    highly_connected_components_clustering(partition.first, parity, min_cut);
    highly_connected_components_clustering(partition.second, parity, min_cut);

    g = merge(partition.first, partition.second); // merge partitions into a single clustered graph
}

} // namespace GV::Clustering