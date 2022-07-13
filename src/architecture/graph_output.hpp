// Contains utility functions for outputting architecture graphs.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_OUTPUT_HPP
#define ARCHITECTURE_GRAPH_OUTPUT_HPP

#include "components.hpp"
#include "graph.hpp"

#include <concepts>
#include <iostream>

namespace architecture
{

void output_structure(std::ostream& os, const structure& v);
void output_dependency(std::ostream& os, const dependency& e);

template <typename OutputStructure = decltype(output_structure),
          typename OutputDependency = decltype(output_dependency)>
requires std::invocable<OutputStructure, std::ostream&, const structure&> &&
    std::invocable<OutputDependency, std::ostream&, const dependency&>
void output_graph(std::ostream& os,
                  const graph& g,
                  OutputStructure out_vertex = output_structure,
                  OutputDependency out_edge = output_dependency)
{
    os << "\n\nvertices:\n\n";
    for (auto v : boost::make_iterator_range(boost::vertices(g)))
    {
        os << "\n--------------------------------\n";
        out_vertex(os, g[v]);
    }

    os << "\n\nedges:\n\n";
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        os << "\n--------------------------------\n";
        os << "from: " << g[boost::source(e, g)].sym.id << '\n';
        os << "to: " << g[boost::target(e, g)].sym.id << '\n';
        out_edge(os, g[e]);
    }
}

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_OUTPUT_HPP
