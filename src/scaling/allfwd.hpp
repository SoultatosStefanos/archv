// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef SCALING_ALLFWD_HPP
#define SCALING_ALLFWD_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/function_property_map.hpp>

namespace scaling
{

class backend;
struct factor;
class factor_repo;
struct scale_vector;
struct config_data;

namespace detail
{
    template < typename Graph, typename FactorCounter >
    class scale_dispatcher;
} // namespace detail

template < typename Graph, typename FactorCounter >
struct scale_map;

} // namespace scaling

#endif // SCALING_ALLFWD_HPP
