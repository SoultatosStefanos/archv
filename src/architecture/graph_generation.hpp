// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_GENERATION_HPP
#define ARCHITECTURE_GRAPH_GENERATION_HPP

#include "graph.hpp"

#include <string_view>

namespace Architecture
{

void generate_graph(Graph& g, const std::string_view json_fpath);

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_GENERATION_HPP
