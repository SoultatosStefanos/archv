// Contains a class responsible for handling camera input events.
// Soultatos Stefanos 2022

#ifndef INPUT_CAMERA_HANDLER_HPP
#define INPUT_CAMERA_HANDLER_HPP

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>

namespace input
{

/***********************************************************
 * Camera Handler                                          *
 ***********************************************************/

// Camera input handler, free look style.
// Wraps the utility class OgreBites::Cameraman, and delegates to it.
// Lock/Unlock: press/release mouse wheel, or press c.
class camera_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using cameraman_type = OgreBites::CameraMan;

    explicit camera_handler(cameraman_type cameraman);
    virtual ~camera_handler() override = default;

    auto cameraman() const -> const cameraman_type& { return m_cameraman; }
    auto cameraman() -> cameraman_type& { return m_cameraman; }

    auto locked() const -> bool { return m_locked; }

    virtual auto frameRendered(const Ogre::FrameEvent& e) -> void override;
    virtual auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    virtual auto keyReleased(const OgreBites::KeyboardEvent& e)
        -> bool override;
    virtual auto mouseMoved(const OgreBites::MouseMotionEvent& e)
        -> bool override;
    virtual auto mousePressed(const OgreBites::MouseButtonEvent& e)
        -> bool override;
    virtual auto mouseReleased(const OgreBites::MouseButtonEvent& e)
        -> bool override;

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
