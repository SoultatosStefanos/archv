#include "sphere.hpp"

#include "topology_enumerator.hpp"
#include "topology_visitor.hpp"

namespace layout
{

sphere::sphere(scale_type scale) : m_sphere { scale }, m_scale { scale } { }

auto sphere::desc() const -> descriptor
{
    return topology_enumerator::sphere_type;
}

auto sphere::accept(const visitor_type& visitor) const -> void
{
    visitor.visit(*this);
}

auto sphere::clone() const -> std::unique_ptr< topology >
{
    return std::make_unique< sphere >(*this);
}

} // namespace layout