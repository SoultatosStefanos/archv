#include "gursoy_atun_layout.hpp"

#include "cube.hpp"
#include "layout_visitor.hpp"
#include "sphere.hpp"
#include "topology_visitor.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace layout::detail
{

class gursoy_atun_visitor : public topology_visitor
{
public:
    using graph = gursoy_atun_layout::graph;
    using weight_map = gursoy_atun_layout::weight_map;
    using position_map = gursoy_atun_layout::position_map;

    gursoy_atun_visitor(const graph& g,
                        weight_map edge_weight,
                        position_map& map)
        : m_g{g}, m_edge_weight{edge_weight}, m_map{map}
    {}

    virtual ~gursoy_atun_visitor() override = default;

    // NOTE: Mind the different overloaded data() calls in each identical
    // implementation.

    virtual void visit(const cube& c) const override
    {
        boost::gursoy_atun_layout(m_g,
                                  c.data(),
                                  boost::make_assoc_property_map(m_map),
                                  boost::weight_map(m_edge_weight));
    }

    virtual void visit(const sphere& s) const override
    {
        boost::gursoy_atun_layout(m_g,
                                  s.data(),
                                  boost::make_assoc_property_map(m_map),
                                  boost::weight_map(m_edge_weight));
    }

private:
    const graph& m_g;
    weight_map m_edge_weight;
    position_map& m_map;
};

} // namespace layout::detail

namespace layout
{

gursoy_atun_layout::gursoy_atun_layout(const graph& g,
                                       const topology& top,
                                       weight_map edge_weight)
{
    top.accept(detail::gursoy_atun_visitor(g, edge_weight, m_map));

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

void gursoy_atun_layout::accept(const layout_visitor& visitor) const
{
    visitor.visit(*this);
}

auto gursoy_atun_layout::clone() const -> std::unique_ptr<layout>
{
    return std::make_unique<gursoy_atun_layout>(*this);
}

} // namespace layout