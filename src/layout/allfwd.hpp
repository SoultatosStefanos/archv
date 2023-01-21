// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef LAYOUT_ALLFWD_HPP
#define LAYOUT_ALLFWD_HPP

#include "misc/boost_property_mapfwd.hpp" // for function_property_map

#include <concepts> // for floating_point

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

template < typename Graph >
class random_layout;

struct backend_config;
using config_data = backend_config;

template < typename Graph >
class layout_factory;

class topology_factory;

namespace detail
{
    template < std::floating_point Coord >
    struct position;

    template < typename Backend, typename Vertex, typename Coord >
    class position_dispatcher;

    template < typename Layout, typename Coord >
    class lposition_dispatcher;

} // namespace detail

template < typename Backend >
using position_map = boost::function_property_map<
    detail::position_dispatcher< Backend, std::size_t, double >,
    std::size_t,
    detail::position< double > >;

template < typename Layout >
using lposition_map = boost::function_property_map<
    detail::lposition_dispatcher< Layout, double >,
    std::size_t,
    detail::position< double > >;

} // namespace layout

#endif // LAYOUT_ALLFWD_HPP
