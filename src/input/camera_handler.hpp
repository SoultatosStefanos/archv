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
    using base = OgreBites::InputListener;

public:
    using cameraman_type = OgreBites::CameraMan;

    explicit camera_handler(cameraman_type cameraman);
    ~camera_handler() override = default;

    auto cameraman() const -> const cameraman_type& { return m_cameraman; }
    auto cameraman() -> cameraman_type& { return m_cameraman; }

    auto locked() const -> bool { return m_locked; }

    auto frameRendered(const Ogre::FrameEvent& evt) -> void override;
    auto keyPressed(const OgreBites::KeyboardEvent& evt) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& evt) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& evt) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& evt) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& evt) -> bool override;

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
