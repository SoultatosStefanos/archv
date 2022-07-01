// Contains some convenience, 3D topology related, typedefs.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <random>
#include <variant>

namespace Layout
{

using Point = typename boost::convex_topology<3>::point_type;

using Cube = boost::cube_topology<std::minstd_rand>;
using Sphere = boost::sphere_topology<std::minstd_rand>;

using Topology = std::variant<Cube, Sphere>;

} // namespace Layout

#endif // LAYOUT_TOPOLOGY_HPP
