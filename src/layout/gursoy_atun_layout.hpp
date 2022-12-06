// Contains a 3D layout class implementing the gursoy atun algorithm.
// Soultatos Stefanos 2022

#ifndef LAYOUT_GURSOY_ATUN_LAYOUT_HPP
#define LAYOUT_GURSOY_ATUN_LAYOUT_HPP

#include "layout.hpp"   // for layout
#include "plugin.hpp"   // for gursoy_atun_id
#include "topology.hpp" // for topology

#include <boost/graph/adjacency_list.hpp>     // for vertices
#include <boost/graph/gursoy_atun_layout.hpp> // for gursoy_atun_layout
#include <cassert>                            // for assert

namespace layout
{

/***********************************************************
 * Gursoy Atun Layout                                      *
 ***********************************************************/

// Assigns a position, at a 3d space, to each graph_type vertex_type.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph_type/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.

template < typename Graph >
class gursoy_atun_layout : public layout< Graph >
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    using base = layout< Graph >;
    using self = gursoy_atun_layout< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;

    template < typename WeightMap >
    gursoy_atun_layout(const graph_type& g, const topology& space, WeightMap);

    ~gursoy_atun_layout() override = default;

    auto id() const -> id_type override { return gursoy_atun_id; }

    auto x(vertex_type v) const -> coord_type override;
    auto y(vertex_type v) const -> coord_type override;
    auto z(vertex_type v) const -> coord_type override;

    auto clone() const -> std::unique_ptr< base > override;

private:
    using position = topology::point_type;
    using position_map = std::unordered_map< vertex_type, position >;

    position_map m_map;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph >
template < typename WeightMap >
inline gursoy_atun_layout< Graph >::gursoy_atun_layout(
    const graph_type& g,
    const topology& space,
    WeightMap edge_weight)
{
    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< graph_type >::edge_descriptor >));

    boost::gursoy_atun_layout(
        g,
        space,
        boost::make_assoc_property_map(m_map),
        boost::weight_map(edge_weight));

    assert(std::all_of(
        boost::vertices(g).first,
        boost::vertices(g).second,
        [this](auto v) { return m_map.contains(v); }));
}

template < typename Graph >
inline auto gursoy_atun_layout< Graph >::x(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[0];
}

template < typename Graph >
inline auto gursoy_atun_layout< Graph >::y(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[1];
}

template < typename Graph >
inline auto gursoy_atun_layout< Graph >::z(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[2];
}

template < typename Graph >
inline auto gursoy_atun_layout< Graph >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template < typename Graph, typename WeightMap >
inline auto make_gursoy_atun_layout(
    const Graph& g,
    const topology& space,
    WeightMap edge_weight)
{
    return gursoy_atun_layout< Graph >(g, space, edge_weight);
}

} // namespace layout

#endif // LAYOUT_GURSOY_ATUN_LAYOUT_HPP
