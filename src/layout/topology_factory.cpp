#include "topology_factory.hpp"

#include "cube.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto topology_factory::make_topology(id_type id, scale_type scale)
    -> pointer
{
    if (id == cube_id)
    {
        return std::make_unique< cube >(scale);
    }
    else if (id == sphere_id)
    {
        return std::make_unique< sphere >(scale);
    }
    else
    {
        assert(!is_topology_plugged_in(id));
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology id: " << id;
        assert(false);
        return nullptr;
    }
}

} // namespace layout