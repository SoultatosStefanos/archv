// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include "components.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <functional>
#include <iostream>

namespace Architecture
{

// ----------------------------- Typedefs ----------------------------------- //

using VertexProperty = Structure;
using EdgeProperty = Dependency;

static_assert(std::is_default_constructible_v<VertexProperty>);
static_assert(std::is_default_constructible_v<EdgeProperty>);

using Graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    VertexProperty,
                                    EdgeProperty>;

using Vertex = Graph::vertex_descriptor;
using Edge = Graph::edge_descriptor;

// -------------------------------------------------------------------------- //

// ---------------------- Utilities ----------------------------------------- //

using OutputVertex = std::function<void(std::ostream&, const VertexProperty&)>;
using OutputEdge = std::function<void(std::ostream&, const EdgeProperty&)>;

void output_vertex(std::ostream& os, const VertexProperty& v);
void output_edge(std::ostream& os, const EdgeProperty& e);

void output_graph(std::ostream& os,
                  const Graph& g,
                  const OutputVertex& out_vertex = output_vertex,
                  const OutputEdge& out_edge = output_edge);

// -------------------------------------------------------------------------- //

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_HPP
