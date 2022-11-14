#include "overlay_manager.hpp"

namespace gui
{

auto overlay_manager::render() const -> void
{
    get_menu_bar().render();
    get_controls_hud().render();
    get_frames_hud().render();
}

auto overlay_manager::visible() const -> bool
{
    return get_menu_bar().visible() or get_controls_hud().visible()
        or get_frames_hud().visible();
}

auto overlay_manager::show() -> void
{
    get_menu_bar().show();
    get_controls_hud().show();
    get_frames_hud().show();
}

auto overlay_manager::hide() -> void
{
    get_menu_bar().hide();
    get_controls_hud().hide();
    get_frames_hud().hide();
}

} // namespace gui