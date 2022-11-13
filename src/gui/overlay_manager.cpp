#include "overlay_manager.hpp"

#include "overlay.hpp"

#include <cassert>

namespace gui
{

auto overlay_manager::manages(id_type id) const -> bool
{
    return m_map.find(id) != std::cend(m_map);
}

auto overlay_manager::num_overlays() const -> size_type
{
    return m_map.size();
}

auto overlay_manager::get(id_type id) const -> const overlay&
{
    assert(manages(id));
    assert(m_map.at(id));
    return *m_map.at(id);
}

auto overlay_manager::get(id_type id) -> overlay&
{
    assert(manages(id));
    assert(m_map.at(id));
    return *m_map.at(id);
}

auto overlay_manager::submit(pointer o) -> void
{
    const auto id = o->id();
    m_map[id] = std::move(o);
    assert(manages(id));
}

auto overlay_manager::withdraw(id_type id) -> void
{
    m_map.erase(id);
    assert(!manages(id));
}

auto show_overlays(overlay_manager& manager) -> void
{
    manager.visit([](auto& o) { o.show(); });
}

auto hide_overlays(overlay_manager& manager) -> void
{
    manager.visit([](auto& o) { o.hide(); });
}

auto render_overlays(overlay_manager& manager) -> void
{
    manager.visit([](const auto& o) { o.render(); });
}

overlay_manager overlays;

} // namespace gui