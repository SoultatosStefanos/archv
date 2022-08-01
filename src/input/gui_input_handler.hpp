// Contains an OIS input handler class responsible for delegating events to
// MyGUI.
// Soultatos Stefanos 2022

#ifndef INPUT_GUI_INPUT_HANDLER_HPP
#define INPUT_GUI_INPUT_HANDLER_HPP

#include <ois/OISJoyStick.h>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>

namespace input
{

// MyGUI input manager adaptor for OIS.
class gui_input_handler : public OIS::KeyListener, public OIS::MouseListener
{
public:
    virtual auto keyPressed(const OIS::KeyEvent& e) -> bool override;
    virtual auto keyReleased(const OIS::KeyEvent& e) -> bool override;

    virtual auto mouseMoved(const OIS::MouseEvent& e) -> bool override;

    virtual auto mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
        -> bool override;

    virtual auto mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
        -> bool override;
};

} // namespace input

#endif // INPUT_GUI_INPUT_HANDLER_HPP
