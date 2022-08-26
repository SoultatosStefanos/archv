// Contains a private module for the position_map header.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_POSITION_MAP_HPP
#define LAYOUT_DETAIL_POSITION_MAP_HPP

#include "layout/core.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace layout::detail
{

struct position
{
    double x, y, z;
};

template < typename Graph, typename WeightMap >
class position_dispatcher
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using vertex = typename boost::graph_traits< graph >::vertex_descriptor;

    explicit position_dispatcher(const core< graph, weight_map >& c) : m_c { c }
    {
    }

    auto operator()(vertex v) const -> position
    {
        const auto& l = m_c.get_layout();
        return { .x = l.x(v), .y = l.y(v), .z = l.z(v) };
    }

private:
    const core< graph, weight_map >& m_c;
};

} // namespace layout::detail

#endif // LAYOUT_DETAIL_POSITION_MAP_HPP
