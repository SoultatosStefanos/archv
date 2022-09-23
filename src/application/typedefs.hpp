// Contains some convenience application level typedefs.
// Soultatos Stefanos 2022

#ifndef APPLICATION_TYPEDEFS_HPP
#define APPLICATION_TYPEDEFS_HPP

#include "architecture/graph.hpp"
#include "dependencies/backend.hpp"
#include "dependencies/weight_map.hpp"
#include "layout/backend.hpp"
#include "layout/position_map.hpp"
#include "rendering/graph_renderer.hpp"

namespace application
{

using weight_map = dependencies::
    weight_map< architecture::graph, architecture::dependency_map >;

using position_map = layout::position_map< architecture::graph, weight_map >;

using layout_backend = layout::backend< architecture::graph, weight_map >;

using graph_renderer = rendering::graph_renderer<
    architecture::graph,
    architecture::id_map,
    position_map,
    weight_map >;

} // namespace application

#endif // APPLICATION_TYPEDEFS_HPP
