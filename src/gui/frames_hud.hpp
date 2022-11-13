// Contains a simple, debug friendly, overlay which counts the fps.
// Soultatos Stefanos 2022

#ifndef GUI_FRAMES_HUD_HPP
#define GUI_FRAMES_HUD_HPP

#include "overlay.hpp"

namespace gui
{

/***********************************************************
 * Frames Hud                                              *
 ***********************************************************/

class frames_hud : public overlay
{
public:
    static constexpr auto type_id = "frames_hud";

    frames_hud() = default;
    ~frames_hud() override = default;

    auto id() const -> id_type override { return type_id; }

    auto visible() const -> bool override { return m_visible; }
    auto show() -> void override { m_visible = true; }
    auto hide() -> void override { m_visible = false; }

    auto render() const -> void override;

private:
    bool m_visible { true };
};

} // namespace gui

#endif // GUI_FRAMES_HUD_HPP
