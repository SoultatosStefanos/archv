#include "topology_factory.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto topology_factory::make_topology(topology_descriptor desc,
                                     topology_scale scale) -> pointer
{
    if (desc == topology_traits<sphere>::desc())
    {
        return std::make_unique<sphere>(scale);
    }
    else if (desc == topology_traits<cube>::desc())
    {
        return std::make_unique<cube>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology type: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace layout