// Contains a simple hud for displaying the application controls.
// Soultatos Stefanos 2022

#ifndef GUI_CONTROLS_HUD_HPP
#define GUI_CONTROLS_HUD_HPP

namespace gui
{

/***********************************************************
 * Controls Hud                                            *
 ***********************************************************/

class controls_hud
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

#endif // GUI_CONTROLS_HUD_HPP
