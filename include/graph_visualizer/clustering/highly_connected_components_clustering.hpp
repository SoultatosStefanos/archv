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
#include <map>
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

        if (boost::get(parity, src) != boost::get(parity, trgt)) // different parity
            edges.insert(boost::edge(src, trgt, g).first);
    }

    assert(edges.size() <= boost::num_edges(g));
    return edges;
}

template <typename MinCutEdgeSet>
inline auto edge_connectivity(const MinCutEdgeSet& min_cut_edges)
{
    return min_cut_edges.size(); // EC = | MinCutEdgeSet |
}

// Make graph filtered view given parity map and partition (true or false vertex set)
template <typename Graph, typename ParityMap, typename MinCutEdgeSet>
inline auto make_parity(const Graph& g, ParityMap parity, const MinCutEdgeSet& min_cut_edges,
                        bool partition)
{
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using EdgePred = std::function<bool(Edge)>;
    using VertexPred = std::function<bool(Vertex)>;
    using Filtered = boost::filtered_graph<Graph, EdgePred, VertexPred>;

    return Filtered{
        g, [&min_cut_edges](auto edge) { return !min_cut_edges.contains(edge); },
        [parity, partition](auto vertex) { return boost::get(parity, vertex) == partition; }};
}

// Divide graph into two disconnected subgraphs, using parity map
template <typename Graph, typename ParityMap, typename MinCutEdgeSet>
inline auto divide(const Graph& g, ParityMap parity, const MinCutEdgeSet& min_cut_edges)
{
    using Partition = std::pair<Graph, Graph>;

    Partition p;
    boost::copy_graph(make_parity(g, parity, min_cut_edges, true), p.first);
    boost::copy_graph(make_parity(g, parity, min_cut_edges, false), p.second);

    assert(boost::num_edges(g) >= boost::num_edges(p.first) + boost::num_edges(p.second));
    assert(boost::num_vertices(g) == boost::num_vertices(p.first) + boost::num_vertices(p.second));
    return p;
}

template <typename MutableGraph, typename MinimumCut, typename ParityMap>
requires std::totally_ordered<typename boost::graph_traits<MutableGraph>::edge_descriptor>
void highly_connected_components_clustering_impl(MutableGraph& g, ParityMap parity,
                                                 MinimumCut min_cut)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using ParityKey = typename boost::property_traits<ParityMap>::key_type;
    using ParityValue = typename boost::property_traits<ParityMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<ParityMap, Vertex>) );

    static_assert(std::is_same_v<ParityKey, Vertex>);
    static_assert(std::is_same_v<ParityValue, bool>);
    static_assert(std::is_invocable_v<MinimumCut, MutableGraph, ParityMap>);

    if (boost::num_edges(g) == 0 or boost::num_vertices(g) < 2) return; // early exit

    min_cut(g, parity); // fill parity map

    const auto min_cut_edges = min_cut_edge_set(g, parity);
    assert(edge_connectivity(min_cut_edges) != 0);

    if (edge_connectivity(min_cut_edges) > boost::num_vertices(g) / 2) return; // break recursion

    auto [g1, g2] = divide(g, parity, min_cut_edges);

    highly_connected_components_clustering_impl(g1, parity, min_cut);
    highly_connected_components_clustering_impl(g2, parity, min_cut);

    g = merge(g1, g2); // merge partition into a single clustered graph
}

} // namespace Details

// Generic Highly Connected Componentes Clustering algorithm
template <typename MutableGraph, typename MinimumCut, typename ParityMap>
requires std::totally_ordered<typename boost::graph_traits<MutableGraph>::edge_descriptor>
inline void highly_connected_components_clustering(MutableGraph& g, ParityMap parity,
                                                   MinimumCut min_cut)
{
    Details::highly_connected_components_clustering_impl(g, parity, min_cut);
}

// Generic Highly Connected Componentes Clustering algorithm, given parity storage as an std map
template <typename MutableGraph, typename MinimumCut>
requires std::totally_ordered<typename boost::graph_traits<MutableGraph>::edge_descriptor>
inline void highly_connected_components_clustering(MutableGraph& g, MinimumCut min_cut)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using ParityStorage = std::map<Vertex, bool>;
    using ParityMap = boost::associative_property_map<ParityStorage>;

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};
    Details::highly_connected_components_clustering_impl(g, parity_map, min_cut);
}

} // namespace GV::Clustering