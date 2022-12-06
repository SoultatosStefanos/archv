// Contains a function for creating a dynamic managed graph position map.
// Soultatos Stefanos 2022

#ifndef LAYOUT_POSITION_MAP_HPP
#define LAYOUT_POSITION_MAP_HPP

#include "detail/position_map.hpp"

#include <boost/property_map/function_property_map.hpp>

namespace layout
{

// A runtime managed 3D vertex-position property map from a layout.
template < typename Backend >
using position_map = boost::function_property_map<
    detail::position_dispatcher< Backend >,
    typename Backend::vertex_type,
    typename detail::position_dispatcher< Backend >::position_type >;

// Creates a runtime managed 3D vertex-position property map from a layout.
template < typename Backend >
inline auto make_position_map(const Backend& b)
{
    return position_map< Backend >(detail::position_dispatcher< Backend >(b));
}

} // namespace layout

#endif // LAYOUT_POSITION_MAP_HPP
