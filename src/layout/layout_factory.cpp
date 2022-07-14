#include "layout_factory.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

auto layout_factory::make_layout(const std::string& type,
                                 const graph& g,
                                 const topology& space)
    -> pointer
{
    if (type == gursoy_atun_type)
    {
        return std::make_unique<gursoy_atun_layout>(g, space);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid layout type: " << type;
        assert(false);
        return nullptr;
    }
}

} // namespace layout