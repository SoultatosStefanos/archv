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
    virtual ~gui_input_listener() override = default;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;
};

} // namespace input

#endif // INPUT_GUI_INPUT_LISTENER_HPP
