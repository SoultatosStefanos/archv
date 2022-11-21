// Contains an input listener responsible for handling combination shortcuts.
// Soultatos Stefanos 2022

#ifndef INPUT_SHORTCUT_INPUT_LISTENER_HPP
#define INPUT_SHORTCUT_INPUT_LISTENER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace undo_redo
{
class command_history;
} // namespace undo_redo

namespace input
{

/***********************************************************
 * Shortcut Handler                                        *
 ***********************************************************/

class shortcut_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using commands_type = undo_redo::command_history;

    explicit shortcut_handler(commands_type& cmds);
    virtual ~shortcut_handler() override = default;

    auto commands() const -> const auto& { return m_cmds; }
    auto commands() -> auto& { return m_cmds; }

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto handle_undo_combination() -> void;
    auto handle_redo_combination() -> void;

    commands_type& m_cmds;

    bool m_lctrl_pressed { false };
    bool m_z_pressed { false };
    bool m_y_pressed { false };
};

} // namespace input

#endif // INPUT_SHORTCUT_INPUT_LISTENER_HPP
