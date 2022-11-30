#include "hud_handler.hpp"

#include "gui/controls_hud.hpp"
#include "gui/frames_hud.hpp"
#include "gui/overlay.hpp"
#include "rendering/minimap_renderer.hpp"

#include <boost/log/trivial.hpp>

namespace input
{

hud_handler::hud_handler(
    controls_hud_type& controls,
    frames_hud_type& frames,
    minimap_renderer_type& minimap_renderer)
: m_ctrls { controls }
, m_frames { frames }
, m_minimap_renderer { minimap_renderer }
{
}

auto hud_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'h')
    {
        BOOST_LOG_TRIVIAL(info) << "toggled controls";
        gui::toggle_show_hide(controls_hud());
    }
    else if (e.keysym.sym == 'f')
    {
        BOOST_LOG_TRIVIAL(info) << "toggled frames";
        gui::toggle_show_hide(frames_hud());
    }
    else if (e.keysym.sym == 'm')
    {
        BOOST_LOG_TRIVIAL(info) << "toggled minimap";
        minimap_renderer().set_visible(!minimap_renderer().visible());
    }

    return base::keyReleased(e);
}

} // namespace input