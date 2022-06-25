// Contains graph output utilities for debugging purposes.
// Soultatos Stefanos 2022.

#ifndef ARCHITECTURE_GRAPH_OUTPUT_HPP
#define ARCHITECTURE_GRAPH_OUTPUT_HPP

#include "graph.hpp"

#include <functional>
#include <iostream>

namespace Architecture
{

using OutputVertex = std::function<void(std::ostream&, const VertexProperty&)>;
using OutputEdge = std::function<void(std::ostream&, const EdgeProperty&)>;

void output_vertex(std::ostream& os, const VertexProperty& v);
void output_edge(std::ostream& os, const EdgeProperty& e);

void output_graph(std::ostream& os,
                  const Graph& g,
                  const OutputVertex& out_vertex = output_vertex,
                  const OutputEdge& out_edge = output_edge);

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_OUTPUT_HPP
