#include "paused_state.hpp"

#include "mygui_input.hpp"
#include "overlay_manager.hpp"

namespace gui
{

paused_state::paused_state(state_machine& machine, state* menu_state)
: m_machine { machine }, m_menu_state { menu_state }
{
}

paused_state::~paused_state()
{
    if (get_pause_menu() != nullptr)
        exit();
}

void paused_state::enter()
{
    m_pause_menu = overlay_manager::get().create_overlay< pause_menu >();
}

void paused_state::exit()
{
    overlay_manager::get().destroy_overlay(m_pause_menu);
}

void paused_state::pause() { }

void paused_state::resume() { }

auto paused_state::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    mygui_dispatch(e);
    // TODO commit transition to main menu
    return true;
}

auto paused_state::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    mygui_dispatch(e);
    return true;
}

auto paused_state::mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool
{
    mygui_dispatch(e);
    return true;
}

auto paused_state::mousePressed(const OgreBites::MouseButtonEvent& e) -> bool
{
    mygui_dispatch(e);
    return true;
}

auto paused_state::mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool
{
    mygui_dispatch(e);
    return true;
}

} // namespace gui