#include "hud_handler.hpp"

namespace input
{

hud_handler::hud_handler(tray_type& tray) : m_tray { tray }
{
}

auto hud_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'f')
        toggle_frame_stats();
    return true;
}

auto hud_handler::toggle_frame_stats() -> void
{
    if (tray().areFrameStatsVisible())
        tray().hideFrameStats();
    else
        tray().showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);
}

} // namespace input