#include "state_event_dispatcher.hpp"

#include "state.hpp"
#include "state_machine.hpp"

namespace states
{

state_event_dispatcher::state_event_dispatcher(const state_machine& sm)
: m_sm { sm }
{
}

void state_event_dispatcher::frameRendered(const Ogre::FrameEvent& e)
{
    if (m_sm.has_active_state())
        m_sm.get_active_state()->frameRendered(e);
}

auto state_event_dispatcher::keyPressed(const OgreBites::KeyboardEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->keyPressed(e)
                                   : false;
}

auto state_event_dispatcher::keyReleased(const OgreBites::KeyboardEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->keyReleased(e)
                                   : false;
}

auto state_event_dispatcher::touchMoved(const OgreBites::TouchFingerEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->touchMoved(e)
                                   : false;
}

auto state_event_dispatcher::touchPressed(const OgreBites::TouchFingerEvent& e)
    -> bool
{

    return m_sm.has_active_state() ? m_sm.get_active_state()->touchPressed(e)
                                   : false;
}

auto state_event_dispatcher::touchReleased(const OgreBites::TouchFingerEvent& e)
    -> bool
{

    return m_sm.has_active_state() ? m_sm.get_active_state()->touchReleased(e)
                                   : false;
}

auto state_event_dispatcher::mouseMoved(const OgreBites::MouseMotionEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->mouseMoved(e)
                                   : false;
}

auto state_event_dispatcher::mouseWheelRolled(
    const OgreBites::MouseWheelEvent& e) -> bool
{
    return m_sm.has_active_state()
        ? m_sm.get_active_state()->mouseWheelRolled(e)
        : false;
}

auto state_event_dispatcher::mousePressed(const OgreBites::MouseButtonEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->mousePressed(e)
                                   : false;
}

auto state_event_dispatcher::mouseReleased(const OgreBites::MouseButtonEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->mouseReleased(e)
                                   : false;
}

auto state_event_dispatcher::textInput(const OgreBites::TextInputEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->textInput(e)
                                   : false;
}

auto state_event_dispatcher::buttonPressed(const OgreBites::ButtonEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->buttonPressed(e)
                                   : false;
}

auto state_event_dispatcher::buttonReleased(const OgreBites::ButtonEvent& e)
    -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->buttonReleased(e)
                                   : false;
}

auto state_event_dispatcher::axisMoved(const OgreBites::AxisEvent& e) -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->axisMoved(e)
                                   : false;
}

} // namespace states