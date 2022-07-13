#include "topology_factory.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
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
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology type: " << type;
        assert(false);
        return topology();
    }
}

} // namespace layout