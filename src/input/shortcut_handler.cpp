#include "shortcut_handler.hpp"

#include "undo_redo/command_history.hpp"

#include <SDL2/SDL_keycode.h>

namespace input
{

shortcut_handler::shortcut_handler(commands_type& cmds) : m_cmds { cmds }
{
}

auto shortcut_handler::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case SDLK_LCTRL:
        m_lctrl_pressed = true;
        break;

    case 'z':
        m_z_pressed = true;
        handle_undo_combination();

        break;

    case 'y':
        m_y_pressed = true;
        handle_redo_combination();
        break;

    default:
        break;
    }

    return true;
}

auto shortcut_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case SDLK_LCTRL:
        m_lctrl_pressed = false;
        break;

    case 'z':
        m_z_pressed = false;
        break;

    case 'y':
        m_y_pressed = false;
        break;

    default:
        break;
    }

    return true;
}

auto shortcut_handler::handle_undo_combination() -> void
{
    if (m_lctrl_pressed and m_z_pressed)
        commands().undo();
}

auto shortcut_handler::handle_redo_combination() -> void
{
    if (m_lctrl_pressed and m_y_pressed)
        commands().redo();
}

} // namespace input