#include "cube.hpp"

#include "topology_visitor.hpp"

namespace layout
{

cube::cube(scale_type scale) : m_cube{scale}, m_scale{scale} {}

void cube::accept(const topology_visitor& visitor) const
{
    visitor.visit(*this);
}

auto cube::clone() const -> std::unique_ptr<topology>
{
    return std::make_unique<cube>(*this);
}

} // namespace layout