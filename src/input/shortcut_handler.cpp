#include "shortcut_handler.hpp"

#include "presentation/def.hpp"

#include <SDL2/SDL_keycode.h>

namespace input
{

shortcut_handler::shortcut_handler(gui_type& overlays) : m_gui { overlays }
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

    case 's':
        m_s_pressed = true;
        handle_save_combination();
        break;

    default:
        break;
    }

    return base::keyPressed(e);
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

    case 's':
        m_s_pressed = false;
        break;

    default:
        break;
    }

    return base::keyReleased(e);
}

auto shortcut_handler::handle_undo_combination() -> void
{
    if (m_lctrl_pressed and m_z_pressed)
        m_gui.get_menu_bar().undo_shortcut();
}

auto shortcut_handler::handle_redo_combination() -> void
{
    if (m_lctrl_pressed and m_y_pressed)
        m_gui.get_menu_bar().redo_shortcut();
}

auto shortcut_handler::handle_save_combination() -> void
{
    if (m_lctrl_pressed and m_s_pressed)
        m_gui.get_menu_bar().save_shortcut();
}

} // namespace input