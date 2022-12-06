// Contains the root gui widget.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include "controls_hud.hpp"    // for controls_hud
#include "frames_hud.hpp"      // for frames_hud
#include "menu_bar.hpp"        // for menu_bar
#include "structure_popup.hpp" // for structure_popup

#include <unordered_set> // for unordered_set

namespace gui
{

/***********************************************************
 * Overlay manager                                         *
 ***********************************************************/

// Manages the lifetime of all gui overlays.
class overlay_manager
{
public:
    using popup_type = structure_popup; // This will be a std::variant later.
    using size_type = std::size_t;

    auto get_menu_bar() const -> const auto& { return m_menu; }
    auto get_menu_bar() -> auto& { return m_menu; }

    auto get_controls_hud() const -> const auto& { return m_ctrls; }
    auto get_controls_hud() -> auto& { return m_ctrls; }

    auto get_frames_hud() const -> const auto& { return m_frames; }
    auto get_frames_hud() -> auto& { return m_frames; }

    auto manages(popup_type* ptr) const -> bool;
    auto num_popups() const -> size_type;

    auto submit(popup_type* ptr) -> void;
    auto withdraw(popup_type* ptr) -> void;

    auto render() const -> void;
    auto visible() const -> bool;
    auto show() -> void;
    auto hide() -> void;

private:
    using popups = std::unordered_set< popup_type* >;

    menu_bar m_menu;
    controls_hud m_ctrls;
    frames_hud m_frames;
    popups m_popups;
};

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
