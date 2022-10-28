#include "cube.hpp"

#include "plugin.hpp"

namespace layout
{

cube::cube(scale_type scale) : m_cube { scale }, m_scale { scale }
{
}

auto cube::id() const -> id_type
{
    return cube_id;
}

auto cube::random_point() const -> point_type
{
    return data().random_point();
}

auto cube::distance(const point_type& p1, const point_type& p2) const
    -> distance_type
{
    return data().distance(p1, p2);
}

auto cube::move_position_toward(
    const point_type& p1, fraction_type fraction, const point_type& p2) const
    -> point_type
{
    return data().move_position_toward(p1, fraction, p2);
}

auto cube::clone() const -> std::unique_ptr< topology >
{
    return std::make_unique< cube >(*this);
}

} // namespace layout