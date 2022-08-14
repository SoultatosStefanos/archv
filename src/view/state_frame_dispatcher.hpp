// Contains a frame listener responsible for delegating frame events to the
// active state of a state machine.
// Soultatos Stefanos 2022

#ifndef VIEW_STATE_FRAME_DISPATCHER_HPP
#define VIEW_STATE_FRAME_DISPATCHER_HPP

#include <OGRE/OgreFrameListener.h>

namespace view
{

class state_machine;

class state_frame_dispatcher : public Ogre::FrameListener
{
public:
    state_frame_dispatcher(const state_machine& sm);
    virtual ~state_frame_dispatcher() = default;

    auto frameStarted(const Ogre::FrameEvent& e) -> bool override;
    auto frameRenderingQueued(const Ogre::FrameEvent& e) -> bool override;
    auto frameEnded(const Ogre::FrameEvent& e) -> bool override;

private:
    const state_machine& m_sm;
};

} // namespace view

#endif // VIEW_STATE_FRAME_DISPATCHER_HPP
