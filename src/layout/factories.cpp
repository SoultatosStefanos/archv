#include "factories.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <typeinfo>
#include <unordered_map>

namespace layout
{

auto layout_factory::resolve_type(const layout& l) -> type_name
{
    using table = std::unordered_map<decltype(typeid(l).name()), type_name>;

    static const auto cache =
        table{{typeid(gursoy_atun_layout).name(), type_name::gursoy_atun}};

    assert(cache.contains(typeid(l).name()));
    return cache.at(typeid(l).name());
}

auto layout_factory::make_layout(type_name type,
                                 const graph& g,
                                 const topology& space) -> pointer
{
    using enum type_name;

    switch (type)
    {
    case gursoy_atun:
        return std::make_unique<gursoy_atun_layout>(g, space);

    default:
        assert(false);
        return nullptr;
    }
}

auto topology_factory::resolve_type(const topology& s) -> type_name
{
    using table = std::unordered_map<decltype(typeid(s).name()), type_name>;

    static const auto cache = table{{typeid(cube).name(), type_name::cube},
                                    {typeid(sphere).name(), type_name::sphere}};

    assert(cache.contains(typeid(s).name()));
    return cache.at(typeid(s).name());
}

auto topology_factory::make_topology(type_name type, scale_type scale)
    -> pointer
{
    switch (type)
    {
    case type_name::cube:
        return std::make_unique<cube>(scale);

    case type_name::sphere:
        return std::make_unique<sphere>(scale);

    default:
        assert(false);
        return nullptr;
    }
}

} // namespace layout