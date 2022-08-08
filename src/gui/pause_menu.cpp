#include "pause_menu.hpp"

#include "view/state_machine.hpp"

namespace gui
{

void pause_menu::enter() { }

void pause_menu::exit() { }

void pause_menu::pause() { }

void pause_menu::resume() { }

auto pause_menu::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'p')
        m_sm.fallback();

    return true;
}

} // namespace gui