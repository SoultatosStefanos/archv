// Contains the color coiding subsystem property map.
// Soultatos Stefanoss 2022

#ifndef COLOR_CODING_COLOR_MAP_HPP
#define COLOR_CODING_COLOR_MAP_HPP

#include "backend.hpp"          // for backend
#include "detail/color_map.hpp" // for color_dispatcher

#include <boost/property_map/function_property_map.hpp> // for function_property_map

namespace color_coding
{

// A runtime managed edge-color property map from a color coding backend.
template < typename Graph, typename DependencyMap >
using color_map = boost::function_property_map<
    detail::color_dispatcher< DependencyMap >,
    typename Graph::edge_descriptor,
    std::optional< std::array< float, 4 > > >;

// Creates a runtime managed edge-color property map from a color coding
// backend.
template < typename Graph, typename DependencyMap >
auto make_color_map(const backend& b, DependencyMap edge_dependency)
{
    return color_map< Graph, DependencyMap >(
        detail::color_dispatcher< DependencyMap > {
            .back = &b, .edge_dependency = edge_dependency });
}

} // namespace color_coding

#endif // COLOR_CODING_COLOR_MAP_HPP
