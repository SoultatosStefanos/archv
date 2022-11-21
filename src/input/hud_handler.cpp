#include "hud_handler.hpp"

#include "gui/controls_hud.hpp"
#include "gui/frames_hud.hpp"
#include "gui/overlay.hpp"

namespace input
{

hud_handler::hud_handler(controls_hud_type& controls, frames_hud_type& frames)
: m_ctrls { controls }, m_frames { frames }
{
}

auto hud_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'h')
        gui::toggle_show_hide(controls_hud());
    else if (e.keysym.sym == 'f')
        gui::toggle_show_hide(frames_hud());

    return base::keyReleased(e);
}

} // namespace input