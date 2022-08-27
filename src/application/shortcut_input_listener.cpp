#include "shortcut_input_listener.hpp"

#include <SDL2/SDL_keycode.h>

namespace application
{

shortcut_input_listener::shortcut_input_listener(menu_bar& bar) : m_menu { bar }
{
}

auto shortcut_input_listener::keyPressed(const OgreBites::KeyboardEvent& e)
    -> bool
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

auto shortcut_input_listener::keyReleased(const OgreBites::KeyboardEvent& e)
    -> bool
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

auto shortcut_input_listener::handle_undo_combination() -> void
{
    if (m_lctrl_pressed and m_z_pressed)
        m_menu.undo_shortcut();
}

auto shortcut_input_listener::handle_redo_combination() -> void
{
    if (m_lctrl_pressed and m_y_pressed)
        m_menu.redo_shortcut();
}

} // namespace application