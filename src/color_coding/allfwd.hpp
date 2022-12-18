// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef COLOR_CODING_ALLFWD_HPP
#define COLOR_CODING_ALLFWD_HPP

#include "misc/boost_property_mapfwd.hpp" // for function_property_map
#include "misc/heterogeneous.hpp"         // for unordered_string_map

#include <array>    // for array
#include <optional> // for optional

namespace color_coding
{

class backend;
class color_repo;

namespace detail
{
    template < typename DependencyMap >
    class color_dispatcher;
} // namespace detail

template < typename Graph, typename DependencyMap >
using color_map = boost::function_property_map<
    detail::color_dispatcher< DependencyMap >,
    typename Graph::edge_descriptor,
    std::optional< std::array< float, 4 > > >;

} // namespace color_coding

#endif // COLOR_CODING_ALLFWD_HPP
