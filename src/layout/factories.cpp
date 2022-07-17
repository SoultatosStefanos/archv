#include "factories.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <typeinfo>

namespace layout
{

auto layout_factory::make_layout(type_name type,
                                 const graph& g,
                                 const topology& space) -> pointer
{
    if (type == typeid(gursoy_atun_layout).name())
    {
        return std::make_unique<gursoy_atun_layout>(g, space);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal)
            << "invalid layout type: " << boost::core::demangle(type.data());
        assert(false);
        return nullptr;
    }
}

auto topology_factory::make_topology(type_name type, scale_type scale)
    -> pointer
{
    if (type == typeid(sphere).name())
    {
        return std::make_unique<sphere>(scale);
    }
    else if (type == typeid(cube).name())
    {
        return std::make_unique<cube>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal)
            << "invalid topology type: " << boost::core::demangle(type.data());
        assert(false);
        return nullptr;
    }
}

} // namespace layout