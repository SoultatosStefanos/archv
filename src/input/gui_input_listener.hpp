// Contains a class which acts as and adaptor for the SDL2 based Ogre::Bites
// Component and the MyGUI input management module.
// Soultatos Stefanos 2022

#ifndef INPUT_GUI_INPUT_LISTENER_HPP
#define INPUT_GUI_INPUT_LISTENER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace input
{

class gui_input_listener : public OgreBites::InputListener
{
public:
    using base = OgreBites::InputListener;
    using keyboard_event = OgreBites::KeyboardEvent;
    using mouse_motion_event = OgreBites::MouseMotionEvent;
    using mouse_button_event = OgreBites::MouseButtonEvent;

    virtual ~gui_input_listener() override = default;

    virtual auto keyPressed(const keyboard_event& e) -> bool override;
    virtual auto keyReleased(const keyboard_event& e) -> bool override;

    virtual auto mouseMoved(const mouse_motion_event& e) -> bool override;
    virtual auto mousePressed(const mouse_button_event& e) -> bool override;
    virtual auto mouseReleased(const mouse_button_event& e) -> bool override;
};

} // namespace input

#endif // INPUT_GUI_INPUT_LISTENER_HPP
