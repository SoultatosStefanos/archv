#include "sphere.hpp"

#include "plugin.hpp"

namespace layout
{

sphere::sphere(scale_type scale) : m_sphere { scale }, m_scale { scale }
{
}

auto sphere::id() const -> id_type
{
    return sphere_id;
}

auto sphere::random_point() const -> point_type
{
    return data().random_point();
}

auto sphere::distance(const point_type& p1, const point_type& p2) const
    -> distance_type
{
    return data().distance(p1, p2);
}

auto sphere::move_position_toward(
    const point_type& p1,
    fraction_type fraction,
    const point_type& p2) const -> point_type
{
    return data().move_position_toward(p1, fraction, p2);
}

auto sphere::clone() const -> std::unique_ptr< topology >
{
    return std::make_unique< sphere >(*this);
}

} // namespace layout