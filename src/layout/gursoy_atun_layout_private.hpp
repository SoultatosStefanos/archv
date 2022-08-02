// Contains a private component of the gursoy_atun_layout.hpp module.
// Soultatos Stefanos 2022

#ifndef LAYOUT_GURSOY_ATUN_LAYOUT_PRIVATE_HPP
#define LAYOUT_GURSOY_ATUN_LAYOUT_PRIVATE_HPP

#include "cube.hpp"
#include "gursoy_atun_layout.hpp"
#include "layout_visitor.hpp"
#include "sphere.hpp"
#include "topology_visitor.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace layout::detail
{

template <typename Graph, typename WeightMap, typename PositionMap>
class gursoy_atun_visitor : public topology_visitor
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using position_map = PositionMap;

    gursoy_atun_visitor(const graph& g,
                        weight_map edge_weight,
                        position_map map)
        : m_g{g}, m_edge_weight{edge_weight}, m_map{map}
    {}

    virtual ~gursoy_atun_visitor() override = default;

    // NOTE: Mind the different overloaded data() calls in each identical
    // implementation.

    virtual void visit(const cube& c) const override
    {
        boost::gursoy_atun_layout(
            m_g, c.data(), m_map, boost::weight_map(m_edge_weight));
    }

    virtual void visit(const sphere& s) const override
    {
        boost::gursoy_atun_layout(
            m_g, s.data(), m_map, boost::weight_map(m_edge_weight));
    }

private:
    const graph& m_g;
    weight_map m_edge_weight;
    position_map m_map;
};

} // namespace layout::detail

#endif // LAYOUT_GURSOY_ATUN_LAYOUT_PRIVATE_HPP
