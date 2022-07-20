#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace layout
{

// NOTE: Mind the different overloaded data() calls in each identical
// implementation.

void gursoy_atun_layout::apply_gursoy_atun::visit(const cube& c) const
{
    boost::gursoy_atun_layout(
        m_g,
        c.data(),
        boost::make_assoc_property_map(m_map),
        boost::weight_map(
            boost::get(&architecture::dependency::cardinality, m_g)));
}

void gursoy_atun_layout::apply_gursoy_atun::visit(const sphere& s) const
{
    boost::gursoy_atun_layout(
        m_g,
        s.data(),
        boost::make_assoc_property_map(m_map),
        boost::weight_map(
            boost::get(&architecture::dependency::cardinality, m_g)));
}

gursoy_atun_layout::gursoy_atun_layout(const graph& g, const topology& top)
{
    top.accept(apply_gursoy_atun(g, m_map));

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

} // namespace layout