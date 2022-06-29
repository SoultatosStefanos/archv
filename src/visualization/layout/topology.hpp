// Contains some convenience, 3D topology related, typedefs.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <concepts>
#include <random>

namespace Visualization
{

template <typename Topology>
concept Convex3DTopology =
    std::is_base_of_v<boost::convex_topology<3>, Topology>;

using Cube = boost::cube_topology<std::minstd_rand>;
using Sphere = boost::sphere_topology<std::minstd_rand>;

} // namespace Visualization

#endif // LAYOUT_TOPOLOGY_HPP
