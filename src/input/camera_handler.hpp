// Contains a class responsible for handling camera input events.
// Soultatos Stefanos 2022

#ifndef INPUT_CAMERA_HANDLER_HPP
#define INPUT_CAMERA_HANDLER_HPP

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>

namespace input
{

// Camera input handler, free look style.
// Wraps the utility class OgreBites::Cameraman, and delegates to it.
// Lock/Unlock: press/release mouse wheel, or press c.
class camera_handler : public OgreBites::InputListener
{
public:
    using cameraman_type = OgreBites::CameraMan;
    using frame_event_type = Ogre::FrameEvent;
    using keyboard_event_type = OgreBites::KeyboardEvent;
    using mouse_motion_event_type = OgreBites::MouseMotionEvent;
    using mouse_button_event_type = OgreBites::MouseButtonEvent;

    explicit camera_handler(cameraman_type cameraman);
    ~camera_handler() override = default;

    auto cameraman() const -> const cameraman_type& { return m_cameraman; }
    auto cameraman() -> cameraman_type& { return m_cameraman; }

    auto locked() const -> bool { return m_locked; }

    auto frameRendered(const frame_event_type& evt) -> void override;
    auto keyPressed(const keyboard_event_type& evt) -> bool override;
    auto keyReleased(const keyboard_event_type& evt) -> bool override;
    auto mouseMoved(const mouse_motion_event_type& evt) -> bool override;
    auto mousePressed(const mouse_button_event_type& evt) -> bool override;
    auto mouseReleased(const mouse_button_event_type& evt) -> bool override;

protected:
    auto toggle_lock_unlock() -> void;
    auto lock() -> void;
    auto unlock() -> void;

private:
    cameraman_type m_cameraman;
    bool m_locked { true };
};

} // namespace input

#endif // INPUT_CAMERA_HANDLER_HPP
