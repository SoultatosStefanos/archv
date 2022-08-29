#include "overlay_manager.hpp"

#include "overlay.hpp"

#include <algorithm>
#include <cassert>

namespace gui
{

auto overlay_manager::submit(overlay* ptr) -> void
{
    assert(ptr);
    m_overlays.push_back(ptr);
}

auto overlay_manager::withdraw(overlay* ptr) -> void
{
    assert(ptr);
    assert(
        std::find(std::begin(m_overlays), std::end(m_overlays), ptr)
        != std::end(m_overlays));

    m_overlays.remove(ptr);
}

auto overlay_manager::draw_all() const -> void
{
    for (auto* ptr : m_overlays)
        ptr->draw();
}

} // namespace gui