#include "topology_factory.hpp"

#include <cassert>

namespace visualization
{

auto topology_factory::make_topology(const std::string& type, double scale)
    -> topology
{
    if (type == sphere_type)
    {
        return sphere(scale);
    }
    else if (type == cube_type)
    {
        return cube(scale);
    }
    else
    {
        assert(false);
        return topology();
    }
}

} // namespace visualization