#include "topology_factory.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <typeinfo>

namespace layout
{

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