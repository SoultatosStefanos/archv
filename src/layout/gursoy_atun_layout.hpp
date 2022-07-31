#ifndef LAYOUT_GURSOY_ATUN_LAYOUT_HPP
#define LAYOUT_GURSOY_ATUN_LAYOUT_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

namespace layout
{

/***********************************************************
 * Gursoy Atun Algorithm                                   *
 ***********************************************************/

// Assigns a position, at a 3d space, to each graph vertex.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.

namespace detail
{
    class gursoy_atun_visitor;
}

class gursoy_atun_layout : public layout
{
public:
    using weight_map = architecture::weight_map;

    static constexpr auto description = "Gursoy Atun";

    gursoy_atun_layout(const graph& g,
                       const topology& space,
                       weight_map edge_weight);

    virtual ~gursoy_atun_layout() override = default;

    virtual auto desc() const -> descriptor override { return description; }

    virtual auto x(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[0];
    }

    virtual auto y(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[1];
    }

    virtual auto z(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[2];
    }

    virtual void accept(const layout_visitor& visitor) const override;

    virtual auto clone() const -> std::unique_ptr<layout> override;

    friend class detail::gursoy_atun_visitor;

private:
    using position_map = std::unordered_map<vertex, topology::point_type>;

    position_map m_map;
};

} // namespace layout

#endif // LAYOUT_GURSOY_ATUN_LAYOUT_HPP
