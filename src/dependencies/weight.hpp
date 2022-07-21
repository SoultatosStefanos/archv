// Contains ant interface for accessing the dependencies edge weight values.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_HPP
#define DEPENDENCIES_WEIGHT_HPP

#include "graph.hpp"

#include <boost/property_map/function_property_map.hpp>
#include <functional>

namespace dependencies
{

using weight = int;

using weight_function = std::function<weight(graph::edge_descriptor)>;

// An edge weight property map adaptor through a uniform function.
using weight_map = boost::
    function_property_map<weight_function, graph::edge_descriptor, weight>;

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_HPP
