#include "core.hpp"

#include "cube.hpp"
#include "sphere.hpp"

namespace layout::detail
{

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    if (desc == topology_traits< cube >::desc())
    {
        return std::make_unique< cube >(scale);
    }
    else if (desc == topology_traits< sphere >::desc())
    {
        return std::make_unique< sphere >(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace layout::detail
