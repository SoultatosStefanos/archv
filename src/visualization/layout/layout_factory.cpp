#include "layout_factory.hpp"

#include <cassert>

namespace visualization
{

auto layout_factory::make_layout(const std::string& type,
                                 const graph& g,
                                 const topology& space)
    -> std::unique_ptr<layout>
{
    if (type == gursoy_atun_type)
    {
        return std::make_unique<gursoy_atun_layout>(g, space);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}

} // namespace visualization