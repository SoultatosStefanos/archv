#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <cassert>

namespace Layout
{

GursoyAtunLayout::GursoyAtunLayout(const Graph& g, const Topology& space)
{
    std::visit(
        [this, &g](const auto& topology) {
            boost::gursoy_atun_layout(
                g,
                topology,
                boost::make_assoc_property_map(m_map),
                boost::weight_map(
                    boost::get(&Architecture::Dependency::cardinality, g)));
        },
        space);

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [this](auto v) { return m_map.contains(v); }));
}

} // namespace Layout