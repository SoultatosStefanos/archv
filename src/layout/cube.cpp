#include "cube.hpp"

#include "plugin.hpp"
#include "topology_visitor.hpp"

namespace layout
{

cube::cube(scale_type scale) : m_cube { scale }, m_scale { scale }
{
}

auto cube::id() const -> id_type
{
    return cube_id;
}

auto cube::accept(const visitor_type& visitor) const -> void
{
    visitor.visit(*this);
}

auto cube::clone() const -> std::unique_ptr< topology >
{
    return std::make_unique< cube >(*this);
}

} // namespace layout