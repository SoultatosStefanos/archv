// Contains a class that is responsibe for delegating input events to the
// current active state.
// Soultatos Stefanos 2022

#ifndef VIEW_STATE_EVENT_DISPATCHER_HPP
#define VIEW_STATE_EVENT_DISPATCHER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace view
{

class state;
class state_machine;

class state_event_dispatcher : public OgreBites::InputListener
{
public:
    explicit state_event_dispatcher(const state_machine& sm);
    virtual ~state_event_dispatcher() override = default;

    void frameRendered(const Ogre::FrameEvent& e);
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto touchMoved(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchPressed(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchReleased(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto textInput(const OgreBites::TextInputEvent& e) -> bool override;
    auto axisMoved(const OgreBites::AxisEvent& e) -> bool override;
    auto buttonPressed(const OgreBites::ButtonEvent& e) -> bool override;
    auto buttonReleased(const OgreBites::ButtonEvent& e) -> bool override;

private:
    const state_machine& m_sm;
};

} // namespace view

#endif // VIEW_STATE_EVENT_DISPATCHER_HPP
