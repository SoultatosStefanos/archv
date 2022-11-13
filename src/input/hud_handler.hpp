// Contains a class responsible for dispatching input to hud elements.
// Soultatos Stefanos 2022

#ifndef INPUT_HUD_HANDLER_HPP
#define INPUT_HUD_HANDLER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace gui
{
class overlay;
} // namespace gui

namespace input
{

/***********************************************************
 * HUD Handler                                             *
 ***********************************************************/

class hud_handler : public OgreBites::InputListener
{
public:
    using overlay_type = gui::overlay;

    hud_handler(overlay_type& controls, overlay_type& frames);
    ~hud_handler() override = default;

    auto controls_hud() const -> const auto& { return m_ctrls; }
    auto controls_hud() -> auto& { return m_ctrls; }

    auto frames_hud() const -> const auto& { return m_frames; }
    auto frames_hud() -> auto& { return m_frames; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    overlay_type& m_ctrls;
    overlay_type& m_frames;
};

} // namespace input

#endif // INPUT_HUD_HANDLER_HPP
