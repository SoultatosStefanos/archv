#include "factories.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace features::layout
{

auto layout_factory::make_layout(const descriptor& desc,
                                 const graph& g,
                                 const topology& space,
                                 weight_map edge_weight) -> pointer
{
    if (desc == gursoy_atun_desc)
    {
        return std::make_unique<gursoy_atun_layout>(g, space, edge_weight);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
        assert(false);
        return nullptr;
    }
}

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    if (desc == cube_desc)
    {
        return std::make_unique<cube>(scale);
    }
    else if (desc == sphere_desc)
    {
        return std::make_unique<sphere>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace features::layout