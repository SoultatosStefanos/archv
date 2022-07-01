#include "topology_factory.hpp"

#include <cassert>

namespace Layout
{

auto TopologyFactory::make_topology(const TopologyType& type, double scale)
    -> Topology
{
    if (type == cube_type)
    {
        return Cube{scale};
    }
    else if (type == sphere_type)
    {
        return Sphere{scale};
    }
    else
    {
        assert(false);
        return {};
    }
}

} // namespace Layout