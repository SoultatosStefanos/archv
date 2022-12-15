// Contains a private module for the position_map header.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_POSITION_MAP_HPP
#define LAYOUT_DETAIL_POSITION_MAP_HPP

#include <concepts> // for floating_point

namespace layout::detail
{

template < std::floating_point Coord >
struct position
{
    Coord x, y, z;
};

template < typename Backend, typename Vertex, typename Coord >
class position_dispatcher
{
public:
    using backend_type = Backend;
    using vertex_type = Vertex;
    using coord_type = Coord;
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

template < typename Layout, typename Coord >
class lposition_dispatcher
{
public:
    using layout_type = Layout;
    using coord_type = Coord;
    using position_type = position< coord_type >;

    explicit lposition_dispatcher(const layout_type& l) : m_lay(&l) { }

    template < typename Vertex >
    auto operator()(Vertex v) const -> position_type
    {
        assert(m_lay);
        return { .x = m_lay->x(v), .y = m_lay->y(v), .z = m_lay->z(v) };
    }

private:
    const layout_type* m_lay { nullptr };
};

} // namespace layout::detail

#endif // LAYOUT_DETAIL_POSITION_MAP_HPP
