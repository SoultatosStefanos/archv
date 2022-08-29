// Contains a function for creating a dynamic managed graph position map.
// Soultatos Stefanos 2022

#ifndef LAYOUT_POSITION_MAP_HPP
#define LAYOUT_POSITION_MAP_HPP

#include "detail/position_map.hpp"
#include "layout.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map/function_property_map.hpp>

namespace layout
{

// A runtime managed 3D vertex-position property map from a layout.
template < typename Graph >
using position_map = boost::function_property_map<
    detail::position_dispatcher< Graph >,
    typename boost::graph_traits< Graph >::vertex_descriptor,
    detail::position >;

// Creates a runtime managed 3D vertex-position property map from a layout.
template < typename Graph >
inline auto make_position_map(const layout< Graph >& l)
{
    return position_map< Graph >(detail::position_dispatcher< Graph >(l));
}

} // namespace layout

#endif // LAYOUT_POSITION_MAP_HPP
