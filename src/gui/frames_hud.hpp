// Contains a simple, debug friendly, overlay which counts the fps.
// Soultatos Stefanos 2022

#ifndef GUI_FRAMES_HUD_HPP
#define GUI_FRAMES_HUD_HPP

namespace gui
{

/***********************************************************
 * Frames Hud                                              *
 ***********************************************************/

class frames_hud
{
public:
    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

private:
    bool m_visible { true };
};

} // namespace gui

#endif // GUI_FRAMES_HUD_HPP
