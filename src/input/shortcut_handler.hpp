// Contains an input listener responsible for handling combination shortcuts.
// Soultatos Stefanos 2022

#ifndef INPUT_SHORTCUT_INPUT_LISTENER_HPP
#define INPUT_SHORTCUT_INPUT_LISTENER_HPP

#include "presentation/deffwd.hpp" // for overlay_manager

#include <OGRE/Bites/OgreInput.h> // for InputListener

namespace input
{

/***********************************************************
 * Shortcut Handler                                        *
 ***********************************************************/

class shortcut_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using gui_type = presentation::overlay_manager;

    explicit shortcut_handler(gui_type& overlays);
    virtual ~shortcut_handler() override = default;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto handle_undo_combination() -> void;
    auto handle_redo_combination() -> void;
    auto handle_save_combination() -> void;

    gui_type& m_gui;

    bool m_lctrl_pressed { false };
    bool m_z_pressed { false };
    bool m_y_pressed { false };
    bool m_s_pressed { false };
};

} // namespace input

#endif // INPUT_SHORTCUT_INPUT_LISTENER_HPP
