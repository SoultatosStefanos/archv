// Contains the dependencies graph for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_GRAPH_HPP
#define DEPENDENCIES_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <functional>

namespace dependencies
{

/***********************************************************
 * Properties                                              *
 ***********************************************************/

using module_id = std::string;
using dependency_type = std::string;

/***********************************************************
 * Graph                                                   *
 ***********************************************************/

// Directed.
using graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    module_id,
                                    dependency_type>;

/***********************************************************
 * Property Maps                                           *
 ***********************************************************/

// NOTE: Using property map typedefs as an alternative static interface to
// templates across the codebase.

// NOTE: Property maps are cheap to copy by design, thus they are passed by
// value.

// Enforces const correctness.
using vertex_id_map =
    decltype(boost::get(boost::vertex_bundle, std::declval<graph>()));

// Enforces const correctness.
using edge_type_map =
    decltype(boost::get(boost::edge_bundle, std::declval<graph>()));

using weight = int;
using weight_function = std::function<weight(graph::edge_descriptor)>;
using weight_map = boost::
    function_property_map<weight_function, graph::edge_descriptor, weight>;

using vertex_table = std::unordered_map<module_id, graph::vertex_descriptor>;
using vertex_map = boost::associative_property_map<vertex_table>;

} // namespace dependencies

#endif // DEPENDENCIES_GRAPH_HPP
