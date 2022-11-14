// Contains a class responsible for dispatching input to hud elements.
// Soultatos Stefanos 2022

#ifndef INPUT_HUD_HANDLER_HPP
#define INPUT_HUD_HANDLER_HPP

#include "gui/overlay.hpp"

#include <OGRE/Bites/OgreInput.h>

namespace input
{

/***********************************************************
 * HUD Handler                                             *
 ***********************************************************/

template < gui::overlay ControlsHUD, gui::overlay FramesHUD >
class hud_handler : public OgreBites::InputListener
{
public:
    using controls_hud_type = ControlsHUD;
    using frames_hud_type = FramesHUD;

    hud_handler(controls_hud_type& controls, frames_hud_type& frames);
    ~hud_handler() override = default;

    auto controls_hud() const -> const auto& { return m_ctrls; }
    auto controls_hud() -> auto& { return m_ctrls; }

    auto frames_hud() const -> const auto& { return m_frames; }
    auto frames_hud() -> auto& { return m_frames; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    controls_hud_type& m_ctrls;
    frames_hud_type& m_frames;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < gui::overlay ControlsHUD, gui::overlay FramesHUD >
inline hud_handler< ControlsHUD, FramesHUD >::hud_handler(
    controls_hud_type& controls, frames_hud_type& frames)
: m_ctrls { controls }, m_frames { frames }
{
}

template < gui::overlay ControlsHUD, gui::overlay FramesHUD >
inline auto hud_handler< ControlsHUD, FramesHUD >::keyReleased(
    const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'h')
        gui::toggle_show_hide(controls_hud());
    else if (e.keysym.sym == 'f')
        gui::toggle_show_hide(frames_hud());
    return true;
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template < gui::overlay ControlsHUD, gui::overlay FramesHUD >
inline auto make_hud_handler(ControlsHUD& controls, FramesHUD& frames)
{
    return hud_handler< ControlsHUD, FramesHUD >(controls, frames);
}

} // namespace input

#endif // INPUT_HUD_HANDLER_HPP
