// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef SCALING_ALLFWD_HPP
#define SCALING_ALLFWD_HPP

#include "misc/boost_property_mapfwd.hpp" // for function_property_map
#include "misc/heterogeneous.hpp"         // for  unordered_string_map

#include <array> // for array

namespace scaling
{

class backend;
struct factor;
class factor_repo;

using scale_t = float;
using scale_vector = std::array< scale_t, 3 >; // (x, y, z)

using config_data = misc::unordered_string_map< factor >;

namespace detail
{
    template < typename Graph, typename FactorCounter >
    class scale_dispatcher;
} // namespace detail

template < typename Graph, typename FactorCounter >
using scale_map = boost::function_property_map<
    detail::scale_dispatcher< Graph, FactorCounter >,
    typename Graph::vertex_descriptor,
    scale_vector >;

} // namespace scaling

#endif // SCALING_ALLFWD_HPP
