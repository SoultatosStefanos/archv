// Contains some convenience application level typedefs.
// Soultatos Stefanos 2022

#ifndef APPLICATION_TYPEDEFS_HPP
#define APPLICATION_TYPEDEFS_HPP

#include "architecture/graph.hpp"
#include "dependencies/weight_map.hpp"
#include "layout/core.hpp"
#include "layout/position_map.hpp"

namespace application
{

// The edge-weight dynamic property map of the architecture graph.
using weight_map = dependencies::
    dynamic_weight_map< architecture::graph, architecture::dependency_map >;

// The vertex-position dynamic property map of the architecture graph.
using position_map = layout::dynamic_position_map< architecture::graph >;

// The layout backend of the architecture graph and its properties.
using layout_core = layout::core< architecture::graph, weight_map >;

} // namespace application

#endif // APPLICATION_TYPEDEFS_HPP
