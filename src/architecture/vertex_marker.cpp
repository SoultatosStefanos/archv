#include "vertex_marker.hpp"

#include <cassert>

namespace architecture
{

auto vertex_marker::vertex(const id_type& id) const -> vertex_type
{
    assert(marks(id));
    return m_map.at(id);
}

auto vertex_marker::num_marked() const -> size_type
{
    return m_map.size();
}

auto vertex_marker::marks(const id_type& id) const -> bool
{
    return m_map.contains(id);
}

auto vertex_marker::mark(id_type id, vertex_type v) -> void
{
    assert(!marks(id));
    m_map[std::move(id)] = std::move(v);
    assert(num_marked() > 0);
}

} // namespace architecture