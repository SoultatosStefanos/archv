// Contains a generic boost::filtered_graph extension, using the std <functional> header
// Soultatos Stefanos 2022

#pragma once

#include <boost/graph/filtered_graph.hpp>
#include <functional>

namespace Clustering {

template <typename Graph>
using EdgePred = std::function<bool(typename boost::graph_traits<Graph>::edge_descriptor)>;

template <typename Graph>
using VertexPred = std::function<bool(typename boost::graph_traits<Graph>::vertex_descriptor)>;

template <typename Graph>
using Filtered = boost::filtered_graph<Graph, EdgePred<Graph>, VertexPred<Graph>>;

// Convenience fuactory for type deduction
template <typename Graph>
inline auto make_filtered(
    const Graph& g, const EdgePred<Graph>& edges = [](auto) { return true; },
    const VertexPred<Graph>& vertices = [](auto) { return true; })
{
    return Filtered<Graph>{g, edges, vertices};
}

} // namespace Clustering