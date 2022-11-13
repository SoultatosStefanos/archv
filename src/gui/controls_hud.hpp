// Contains a simple hud for displaying the application controls.
// Soultatos Stefanos 2022

#ifndef GUI_CONTROLS_HUD_HPP
#define GUI_CONTROLS_HUD_HPP

#include "overlay.hpp"

namespace gui
{

class controls_hud : public overlay
{
public:
    static constexpr auto type_id = "controls_hud";

    controls_hud() = default;
    ~controls_hud() override = default;

    auto id() const -> id_type override { return type_id; }

    auto visible() const -> bool override { return m_visible; }
    auto show() -> void override { m_visible = true; }
    auto hide() -> void override { m_visible = false; }

    auto render() const -> void override;

private:
    bool m_visible { true };
};

} // namespace gui

#endif // GUI_CONTROLS_HUD_HPP
