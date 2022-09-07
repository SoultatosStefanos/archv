// Contains a private component of the gursoy_atun_layout.hpp module.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_GURSOY_ATUN_LAYOUT_HPP
#define LAYOUT_DETAIL_GURSOY_ATUN_LAYOUT_HPP

#include "layout/cube.hpp"
#include "layout/gursoy_atun_layout.hpp"
#include "layout/layout_visitor.hpp"
#include "layout/sphere.hpp"
#include "layout/topology_visitor.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace layout::detail
{

template < typename Graph, typename WeightMap, typename PositionMap >
class gursoy_atun_visitor : public topology_visitor
{
public:
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using position_map_type = PositionMap;

    gursoy_atun_visitor(
        const graph_type& g, weight_map_type edge_weight, position_map_type map)
    : m_g { g }, m_edge_weight { edge_weight }, m_map { map }
    {
    }

    virtual ~gursoy_atun_visitor() override = default;

    // NOTE: Mind the different overloaded data() calls in each identical
    // implementation.

    virtual auto visit(const cube& c) const  -> void override
    {
        boost::gursoy_atun_layout(
            m_g, c.data(), m_map, boost::weight_map(m_edge_weight));
    }

    virtual auto visit(const sphere& s) const  -> void override
    {
        boost::gursoy_atun_layout(
            m_g, s.data(), m_map, boost::weight_map(m_edge_weight));
    }

private:
    const graph_type& m_g;
    weight_map_type m_edge_weight;
    position_map_type m_map;
};

} // namespace layout::detail

#endif // LAYOUT_DETAIL_GURSOY_ATUN_LAYOUT_HPP
