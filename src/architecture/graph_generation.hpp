// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_GENERATION_HPP
#define ARCHITECTURE_GRAPH_GENERATION_HPP

#include "graph.hpp"

#include <istream>

namespace Architecture
{

using JsonArchive = std::istream;
void generate_graph(Graph& g, JsonArchive& from);

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_GENERATION_HPP
