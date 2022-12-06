// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_ALLFWD_HPP
#define WEIGHTS_ALLFWD_HPP

#include "misc/boost_property_mapfwd.hpp" // for function_property_map
#include "misc/heterogeneous.hpp"         // for unordered_string_map

namespace weights
{

class backend;
class weight_repo;

using config_data = misc::unordered_string_map< int >;

namespace detail
{
    template < typename Graph, typename DependencyMap >
    class weight_dispatcher;
} // namespace detail

template < typename Graph, typename DependencyMap >
using weight_map = boost::function_property_map<
    detail::weight_dispatcher< Graph, DependencyMap >,
    typename Graph ::edge_descriptor,
    int >;

} // namespace weights

#endif // WEIGHTS_ALLFWD_HPP
