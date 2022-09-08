#include "topology_factory.hpp"

#include "cube.hpp"
#include "sphere.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

static_assert(cube::description == topology_enumerator::cube_desc);
static_assert(sphere::description == topology_enumerator::sphere_desc);

static_assert(is_topology_enumerated(topology_enumerator::cube_desc));
static_assert(is_topology_enumerated(topology_enumerator::sphere_desc));

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    auto ptr = make_topology_impl(desc, scale);
    assert(ptr->desc() == desc);
    assert(is_topology_enumerated(ptr->desc()));
    return ptr;
}

auto topology_factory::make_topology_impl(
    const descriptor& desc, scale_type scale) -> pointer
{
    if (desc == enumerator::cube_desc)
    {
        return std::make_unique< cube >(scale);
    }
    else if (desc == enumerator::sphere_desc)
    {
        return std::make_unique< sphere >(scale);
    }
    else
    {
        assert(!is_topology_enumerated(desc));
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace layout