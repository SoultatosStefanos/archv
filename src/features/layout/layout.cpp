#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace features::layout
{

// NOTE: Mind the different overloaded data() calls in each identical
// implementation.

void gursoy_atun_layout::apply_gursoy_atun::visit(const cube& c) const
{
    // FIXME
    boost::gursoy_atun_layout(
        m_g,
        c.data(),
        boost::make_assoc_property_map(m_map),
        boost::weight_map(dependencies::weight_map([](auto) { return 1; })));
}

void gursoy_atun_layout::apply_gursoy_atun::visit(const sphere& s) const
{
    // FIXME
    boost::gursoy_atun_layout(
        m_g,
        s.data(),
        boost::make_assoc_property_map(m_map),
        boost::weight_map(dependencies::weight_map([](auto) { return 1; })));
}

gursoy_atun_layout::gursoy_atun_layout(const graph& g, const topology& top)
{
    top.accept(apply_gursoy_atun(g, m_map));

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

} // namespace features::layout