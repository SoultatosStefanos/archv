// Contains the root gui widget.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include "controls_hud.hpp"
#include "frames_hud.hpp"
#include "menu_bar.hpp"

namespace gui
{

/***********************************************************
 * Overlay manager                                         *
 ***********************************************************/

// Manages the lifetime of all gui overlays.
// TODO Submit/withdraw structure_popup
class overlay_manager
{
public:
    auto get_menu_bar() const -> const auto& { return m_menu; }
    auto get_menu_bar() -> auto& { return m_menu; }

    auto get_controls_hud() const -> const auto& { return m_ctrls; }
    auto get_controls_hud() -> auto& { return m_ctrls; }

    auto get_frames_hud() const -> const auto& { return m_frames; }
    auto get_frames_hud() -> auto& { return m_frames; }

    auto render() const -> void;
    auto visible() const -> bool;
    auto show() -> void;
    auto hide() -> void;

private:
    menu_bar m_menu;
    controls_hud m_ctrls;
    frames_hud m_frames;
};

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
