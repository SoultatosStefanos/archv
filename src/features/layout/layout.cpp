#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace features::layout
{

// NOTE: Mind the different overloaded data() calls in each identical
// implementation.

void gursoy_atun_layout::gursoy_atun_visitor::visit(const cube& c) const
{
    boost::gursoy_atun_layout(m_g,
                              c.data(),
                              boost::make_assoc_property_map(m_map),
                              boost::weight_map(m_edge_weight));
}

void gursoy_atun_layout::gursoy_atun_visitor::visit(const sphere& s) const
{
    boost::gursoy_atun_layout(m_g,
                              s.data(),
                              boost::make_assoc_property_map(m_map),
                              boost::weight_map(m_edge_weight));
}

gursoy_atun_layout::gursoy_atun_layout(const graph& g,
                                       const topology& top,
                                       weight_map edge_weight)
{
    top.accept(gursoy_atun_visitor(g, edge_weight, m_map));

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

} // namespace features::layout