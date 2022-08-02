// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include "symbols.hpp"

#include <boost/graph/adjacency_list.hpp>

namespace architecture
{

/***********************************************************
 * Properties                                              *
 ***********************************************************/

using dependency_type = std::string;

/***********************************************************
 * Graph                                                   *
 ***********************************************************/

using graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    structure::id_type,
                                    dependency_type>;

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_HPP
