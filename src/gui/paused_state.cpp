#include "paused_state.hpp"

#include "view/state_machine.hpp"

#include "mygui_input.hpp"

namespace gui
{

paused_state::paused_state(
    state_machine& machine,
    state* menu_state,
    layout_options layouts,
    topology_options topologies,
    scale_options scales)
: m_machine { machine }
, m_menu_state { menu_state }
, m_layouts { std::move(layouts) }
, m_topologies { std::move(topologies) }
, m_scales { std::move(scales) }
{
}

void paused_state::enter()
{
    m_gui = std::make_unique< pause_menu >(m_layouts, m_topologies, m_scales);
}

void paused_state::exit() { m_gui.reset(); }

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