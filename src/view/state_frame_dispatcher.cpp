#include "state_frame_dispatcher.hpp"

#include "state.hpp"
#include "state_machine.hpp"

namespace view
{

state_frame_dispatcher::state_frame_dispatcher(const state_machine& sm)
: m_sm { sm }
{
}

auto state_frame_dispatcher::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->frameStarted(e)
                                   : false;
}

auto state_frame_dispatcher::frameRenderingQueued(const Ogre::FrameEvent& e)
    -> bool
{
    return m_sm.has_active_state()
        ? m_sm.get_active_state()->frameRenderingQueued(e)
        : false;
}

auto state_frame_dispatcher::frameEnded(const Ogre::FrameEvent& e) -> bool
{
    return m_sm.has_active_state() ? m_sm.get_active_state()->frameEnded(e)
                                   : false;
}

} // namespace view