// Contains some convenience, 3D topology related, typedefs.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <random>
#include <variant>

namespace layout
{

using point = typename boost::convex_topology<3>::point_type;

using cube = boost::cube_topology<std::minstd_rand>;
using sphere = boost::sphere_topology<std::minstd_rand>;

using topology = std::variant<cube, sphere>;

} // namespace layout

#endif // LAYOUT_TOPOLOGY_HPP
