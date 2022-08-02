#include "sphere.hpp"

#include "topology_visitor.hpp"

namespace layout
{

sphere::sphere(scale_type scale) : m_sphere { scale }, m_scale { scale } { }

void sphere::accept(const topology_visitor& visitor) const
{
    visitor.visit(*this);
}

auto sphere::clone() const -> std::unique_ptr< topology >
{
    return std::make_unique< sphere >(*this);
}

} // namespace layout