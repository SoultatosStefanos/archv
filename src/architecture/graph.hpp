// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include "components.hpp"

#include <boost/graph/adjacency_list.hpp>

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

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_HPP
