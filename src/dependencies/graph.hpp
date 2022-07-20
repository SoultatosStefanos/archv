// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_GRAPH_HPP
#define DEPENDENCIES_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>

namespace dependencies
{

using id_type = std::string;

using dependency_type = std::string;

using graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    id_type,
                                    dependency_type>;

} // namespace dependencies

#endif // DEPENDENCIES_GRAPH_HPP
