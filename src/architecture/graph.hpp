// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include "components.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <functional>
#include <iostream>

namespace architecture
{

// ----------------------------- Typedefs ----------------------------------- //

static_assert(std::is_default_constructible_v<structure>);
static_assert(std::is_default_constructible_v<dependency>);

using graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    structure,
                                    dependency>;

// -------------------------------------------------------------------------- //

// ---------------------- Utilities ----------------------------------------- //

void output_structure(std::ostream& os, const structure& v);
void output_dependency(std::ostream& os, const dependency& e);

using output_vertex = std::function<void(std::ostream&, const structure&)>;
using output_edge = std::function<void(std::ostream&, const dependency&)>;

void output_graph(std::ostream& os,
                  const graph& g,
                  const output_vertex& out_vertex = output_structure,
                  const output_edge& out_edge = output_dependency);

// -------------------------------------------------------------------------- //

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_HPP
