// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef LAYOUT_ALLFWD_HPP
#define LAYOUT_ALLFWD_HPP

namespace layout
{

template < typename Graph, typename WeightMap >
class core;

class topology_visitor;

class topology;
class cube;
class sphere;

template < typename Graph >
class layout_visitor;

template < typename Graph >
class layout;

template < typename Graph >
class gursoy_atun_layout;

struct config_data;

} // namespace layout

#endif // LAYOUT_ALLFWD_HPP
