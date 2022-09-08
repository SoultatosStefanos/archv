// Contains a private module for the position_map header.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_POSITION_MAP_HPP
#define LAYOUT_DETAIL_POSITION_MAP_HPP

#include "layout/backend.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <concepts>

namespace layout::detail
{

template < std::floating_point Coord >
struct position
{
    Coord x, y, z;
};

template < typename Graph, typename WeightMap >
class position_dispatcher
{
public:
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using vertex_type = typename graph_type::vertex_descriptor;
    using backend_type = backend< graph_type, weight_map_type >;
    using coord_type = typename backend_type::layout_type::coord_type;
    using position_type = position< coord_type >;

    explicit position_dispatcher(const backend_type& b) : m_backend { &b } { }

    auto operator()(vertex_type v) const -> position_type
    {
        const auto& l = m_backend->get_layout();
        return { .x = l.x(v), .y = l.y(v), .z = l.z(v) };
    }

private:
    const backend_type* m_backend { nullptr };
};

} // namespace layout::detail

#endif // LAYOUT_DETAIL_POSITION_MAP_HPP
