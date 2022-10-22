// Contains an input listener responsible for handling gui shortcuts.
// Soultatos Stefanos 2022

#ifndef INPUT_SHORTCUT_INPUT_LISTENER_HPP
#define INPUT_SHORTCUT_INPUT_LISTENER_HPP

#include "gui/allfwd.hpp"

#include <OGRE/Bites/OgreInput.h>

namespace input
{

class shortcut_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using gui_type = gui::gui;

    explicit shortcut_handler(gui_type& ui);
    virtual ~shortcut_handler() override = default;

    auto get_gui() const -> const auto& { return m_gui; }
    auto get_gui() -> auto& { return m_gui; }

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto handle_undo_combination() -> void;
    auto handle_redo_combination() -> void;

    gui_type& m_gui;

    bool m_lctrl_pressed { false };
    bool m_z_pressed { false };
    bool m_y_pressed { false };
};

} // namespace input

#endif // INPUT_SHORTCUT_INPUT_LISTENER_HPP
