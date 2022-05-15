// Contains Minimum Spanning Tree generic algorithms and boost adapters
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename Graph, typename PredecessorMap>
auto prim_minimum_spanning_tree_impl(const Graph& g, PredecessorMap p_map) -> Graph
{
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using PredecessorMapValue = typename boost::property_traits<PredecessorMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Graph>) );

    static_assert(std::is_trivially_copyable_v<PredecessorMap>);
    static_assert(std::is_same_v<Vertex, PredecessorMapValue>);

    boost::prim_minimum_spanning_tree(g, p_map);

    // Build MST
    Graph mst;

    const auto& [begin, end] = boost::vertices(g);

    for (auto iter = begin; iter != end; ++iter)
        if (boost::get(p_map, *iter) != *iter) // avoid root object edge to itself
            boost::add_edge(*iter, boost::get(p_map, *iter), mst);

    return mst;
}

} // namespace Details

// Prim minimum spanning tree generic boost adaptor
template <typename Graph, typename PredecessorMap>
auto prim_minimum_spanning_tree(const Graph& g, PredecessorMap p_map) -> Graph
{
    return Details::prim_minimum_spanning_tree_impl(g, p_map);
}

// Prim minimum spanning tree generic boost adaptor, with default vector predecessor map
template <typename Graph>
auto prim_minimum_spanning_tree(const Graph& g) -> Graph
{
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using PredecessorMap = std::vector<Vertex>;

    PredecessorMap p_map(boost::num_vertices(g));
    return Details::prim_minimum_spanning_tree_impl(g, &p_map[0]);
}

// TODO Add Kruskal adapter

} // namespace GV::Clustering