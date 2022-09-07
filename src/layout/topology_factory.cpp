#include "topology_factory.hpp"

#include "cube.hpp"
#include "sphere.hpp"
#include "topology_enumerator.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    auto ptr = make_topology_impl(desc, scale);
    assert(ptr->desc() == desc);
    assert(topology_enumerator::enumerates(ptr->desc()));
    return ptr;
}

auto topology_factory::make_topology_impl(
    const descriptor& desc, scale_type scale) -> pointer
{
    if (desc == topology_enumerator::cube_type)
    {
        return std::make_unique< cube >(scale);
    }
    else if (desc == topology_enumerator::sphere_type)
    {
        return std::make_unique< sphere >(scale);
    }
    else
    {
        assert(!topology_enumerator::enumerates(desc));
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace layout