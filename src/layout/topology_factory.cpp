#include "topology_factory.hpp"

#include "cube.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto topology_factory::make_topology(const identifier& id, scale_type scale)
    -> pointer
{
    if (id == topology_plugin::cube_id)
    {
        return std::make_unique< cube >(scale);
    }
    else if (id == topology_plugin::sphere_id)
    {
        return std::make_unique< sphere >(scale);
    }
    else
    {
        assert(!topology_plugin::enumerates(id));
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology id: " << id;
        assert(false);
        return nullptr;
    }
}

} // namespace layout