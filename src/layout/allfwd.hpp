// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef LAYOUT_ALLFWD_HPP
#define LAYOUT_ALLFWD_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/function_property_map.hpp>

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
struct config_data;

namespace detail
{
    template < typename Graph, typename WeightMap >
    class position_dispatcher;
} // namespace detail

template < typename Graph, typename WeightMap >
struct position_map;

} // namespace layout

#endif // LAYOUT_ALLFWD_HPP
