#include "layout_factory.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto layout_factory::make_layout(layout_descriptor desc,
                                 const graph& g,
                                 const topology& space) -> pointer
{
    if (desc == layout_traits<gursoy_atun_layout>::desc())
    {
        return std::make_unique<gursoy_atun_layout>(g, space);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid layout type: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace layout