// Contains a class responsible for dispatching input to hud elements.
// Soultatos Stefanos 2022

#ifndef INPUT_HUD_HANDLER_HPP
#define INPUT_HUD_HANDLER_HPP

#include "gui/allfwd.hpp"

#include <OGRE/Bites/OgreInput.h>

namespace rendering
{
class minimap_renderer;
} // namespace rendering

namespace input
{

/***********************************************************
 * HUD Handler                                             *
 ***********************************************************/

// TODO Toggle minimap
class hud_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using controls_hud_type = gui::controls_hud;
    using frames_hud_type = gui::frames_hud;
    using minimap_renderer_type = rendering::minimap_renderer;

    hud_handler(
        controls_hud_type& controls,
        frames_hud_type& frames,
        minimap_renderer_type& minimap_renderer);

    ~hud_handler() override = default;

    auto controls_hud() const -> const auto& { return m_ctrls; }
    auto controls_hud() -> auto& { return m_ctrls; }

    auto frames_hud() const -> const auto& { return m_frames; }
    auto frames_hud() -> auto& { return m_frames; }

    auto minimap_renderer() const -> const auto& { return m_minimap_renderer; }
    auto minimap_renderer() -> auto& { return m_minimap_renderer; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    controls_hud_type& m_ctrls;
    frames_hud_type& m_frames;
    minimap_renderer_type& m_minimap_renderer;
};

} // namespace input

#endif // INPUT_HUD_HANDLER_HPP
