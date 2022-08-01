#include "gui_input_handler.hpp"

#include <MYGUI/MyGUI.h>

namespace input
{

auto gui_input_handler::keyPressed(const OIS::KeyEvent& e) -> bool
{
    MyGUI::InputManager::getInstance().injectKeyPress(
        MyGUI::KeyCode::Enum(e.key), e.text);

    return true;
}

auto gui_input_handler::keyReleased(const OIS::KeyEvent& e) -> bool
{
    MyGUI::InputManager::getInstance().injectKeyRelease(
        MyGUI::KeyCode::Enum(e.key));

    return true;
}

auto gui_input_handler::mouseMoved(const OIS::MouseEvent& e) -> bool
{
    MyGUI::InputManager::getInstance().injectMouseMove(
        e.state.X.abs, e.state.Y.abs, e.state.Z.abs);

    return true;
}

auto gui_input_handler::mousePressed(const OIS::MouseEvent& e,
                                     OIS::MouseButtonID id) -> bool
{
    MyGUI::InputManager::getInstance().injectMousePress(
        e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));

    return true;
}

auto gui_input_handler::mouseReleased(const OIS::MouseEvent& e,
                                      OIS::MouseButtonID id) -> bool
{
    MyGUI::InputManager::getInstance().injectMouseRelease(
        e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));

    return true;
}

} // namespace input