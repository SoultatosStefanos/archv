// Contains a private module for the position_map header.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_POSITION_MAP_HPP
#define LAYOUT_DETAIL_POSITION_MAP_HPP

#include "layout/layout.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace layout::detail
{

struct position
{
    double x, y, z;
};

template < typename Graph >
class position_dispatcher
{
public:
    using graph = Graph;
    using vertex = typename boost::graph_traits< graph >::vertex_descriptor;

    explicit position_dispatcher(const layout< graph >& l) : m_l { &l } { }

    auto operator()(vertex v) const -> position
    {
        return { .x = m_l->x(v), .y = m_l->y(v), .z = m_l->z(v) };
    }

private:
    const layout< graph >* m_l { nullptr };
};

} // namespace layout::detail

#endif // LAYOUT_DETAIL_POSITION_MAP_HPP
