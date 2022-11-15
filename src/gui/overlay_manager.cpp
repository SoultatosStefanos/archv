#include "overlay_manager.hpp"

#include <cassert>
#include <ranges>

namespace gui
{

auto overlay_manager::manages(popup_type* ptr) const -> bool
{
    assert(ptr);
    return m_popups.contains(ptr);
}

auto overlay_manager::num_popups() const -> size_type
{
    return m_popups.size();
}

auto overlay_manager::submit(popup_type* ptr) -> void
{
    assert(ptr);
    m_popups.insert(ptr);
    assert(manages(ptr));
    assert(num_popups() > 0);
}

auto overlay_manager::withdraw(popup_type* ptr) -> void
{
    assert(ptr);
    m_popups.erase(ptr);
    assert(!manages(ptr));
    assert(num_popups() < m_popups.max_size());
}

auto overlay_manager::render() const -> void
{
    using std::ranges::for_each;
    using std::ranges::views::all;

    get_menu_bar().render();
    get_controls_hud().render();
    get_frames_hud().render();
    for_each(all(m_popups), [](auto* p) { p->render(); });
}

auto overlay_manager::visible() const -> bool
{
    using std::ranges::any_of;
    using std::ranges::views::all;

    return get_menu_bar().visible() or get_controls_hud().visible()
        or get_frames_hud().visible()
        or any_of(all(m_popups), [](auto* p) { return p->visible(); });
}

auto overlay_manager::show() -> void
{
    using std::ranges::for_each;
    using std::ranges::views::all;

    get_menu_bar().show();
    get_controls_hud().show();
    get_frames_hud().show();
    for_each(all(m_popups), [](auto* p) { p->show(); });
}

auto overlay_manager::hide() -> void
{
    using std::ranges::for_each;
    using std::ranges::views::all;

    get_menu_bar().hide();
    get_controls_hud().hide();
    get_frames_hud().hide();
    for_each(all(m_popups), [](auto* p) { p->hide(); });
}

} // namespace gui