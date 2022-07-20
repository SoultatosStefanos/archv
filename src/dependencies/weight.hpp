// Contains some useful typedefs to drive the dynamic edge weight management
// subsystem.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_HPP
#define DEPENDENCIES_WEIGHT_HPP

#include "graph.hpp"

#include <boost/property_map/function_property_map.hpp>

namespace dependencies
{

using weight_type = int;

using weight_function = std::function<weight_type(graph::vertex_descriptor)>;

using weight_map = boost::function_property_map<weight_function,
                                                graph::vertex_descriptor,
                                                weight_type>;

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_HPP
