#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace layout
{

gursoy_atun_layout::gursoy_atun_layout(const graph& g, const topology& space)
{
    std::visit(
        [this, &g](const auto& topology) {
            boost::gursoy_atun_layout(
                g,
                topology,
                boost::make_assoc_property_map(m_map),
                boost::weight_map(
                    boost::get(&architecture::dependency::cardinality, g)));
        },
        space);

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

} // namespace layout