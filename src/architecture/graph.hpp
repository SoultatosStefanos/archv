// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp> // for adjacency_list
#include <string>                         // for string

namespace architecture
{

/***********************************************************
 * Properties                                              *
 ***********************************************************/

using symbol_type = std::string;
using dependency_type = std::string;

/***********************************************************
 * Graph                                                   *
 ***********************************************************/

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    symbol_type,
    dependency_type >;

/***********************************************************
 * Property Maps                                           *
 ***********************************************************/

using id_map
    = decltype(boost::get(boost::vertex_bundle, std::declval< graph >()));

using dependency_map
    = decltype(boost::get(boost::edge_bundle, std::declval< graph >()));

// Parallel edges are allowed.
static_assert(std::is_same_v<
              typename boost::parallel_edge_traits< boost::vecS >::type,
              boost::allow_parallel_edge_tag >);

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_HPP
