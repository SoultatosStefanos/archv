// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef LAYOUT_ALLFWD_HPP
#define LAYOUT_ALLFWD_HPP

namespace boost
{

template < typename Func, typename Key, typename Ret >
class function_property_map;

} // namespace boost

namespace layout
{

template < typename Graph, typename WeightMap >
class backend;

class topology;
class cube;
class sphere;

template < typename Graph >
class layout;

template < typename Graph >
class gursoy_atun_layout;

struct backend_config;
using config_data = backend_config;

template < typename Graph >
class layout_factory;

class topology_factory;

namespace detail
{
    template < typename backend >
    class position_dispatcher;
} // namespace detail

template < typename Backend >
using position_map = boost::function_property_map<
    detail::position_dispatcher< Backend >,
    typename Backend::vertex_type,
    typename detail::position_dispatcher< Backend >::position_type >;

} // namespace layout

#endif // LAYOUT_ALLFWD_HPP
