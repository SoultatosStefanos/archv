// Contains an input listener responsible for handling gui shortcuts.
// Soultatos Stefanos 2022

#ifndef APPLICATION_SHORTCUT_INPUT_HANDLER_HPP
#define APPLICATION_SHORTCUT_INPUT_HANDLER_HPP

#include "gui/editor.hpp"

#include <OGRE/Bites/OgreInput.h>

namespace application
{

class shortcut_input_listener : public OgreBites::InputListener
{
public:
    using editor = gui::editor;

    explicit shortcut_input_listener(editor& bar);
    virtual ~shortcut_input_listener() override = default;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto handle_undo_combination() -> void;
    auto handle_redo_combination() -> void;

    editor& m_menu;

    bool m_lctrl_pressed { false };
    bool m_z_pressed { false };
    bool m_y_pressed { false };
};

} // namespace application

#endif // APPLICATION_SHORTCUT_INPUT_HANDLER_HPP
