// Contains a function for creating a dynamic managed graph position map.
// Soultatos Stefanos 2022

#ifndef LAYOUT_POSITION_MAP_HPP
#define LAYOUT_POSITION_MAP_HPP

#include "backend.hpp"
#include "detail/position_map.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map/function_property_map.hpp>

namespace layout
{

// A runtime managed 3D vertex-position property map from a layout.
template < typename Graph, typename WeightMap >
using position_map = boost::function_property_map<
    detail::position_dispatcher< Graph, WeightMap >,
    typename boost::graph_traits< Graph >::vertex_descriptor,
    typename detail::position_dispatcher< Graph, WeightMap >::position_type >;

// Creates a runtime managed 3D vertex-position property map from a layout.
template < typename Graph, typename WeightMap >
inline auto make_position_map(const backend< Graph, WeightMap >& b)
{
    return position_map< Graph, WeightMap >(
        detail::position_dispatcher< Graph, WeightMap >(b));
}

} // namespace layout

#endif // LAYOUT_POSITION_MAP_HPP
