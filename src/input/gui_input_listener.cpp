#include "gui_input_listener.hpp"

#include <MYGUI/MyGUI.h>

namespace input
{

using namespace OgreBites;

namespace
{
    inline auto native_manager() -> auto&
    {
        return MyGUI::InputManager::getInstance();
    }

    // TODO Test this
    inline auto translate(OgreBites::Keycode code) -> MyGUI::KeyCode
    {
        return MyGUI::KeyCode::Enum(code);
    }

    inline auto translate(unsigned char mouse_button) -> MyGUI::MouseButton
    {
        return MyGUI::MouseButton::Enum(mouse_button - 1); // Check enums.
    }

} // namespace

auto gui_input_listener::keyPressed(const KeyboardEvent& e) -> bool
{
    native_manager().injectKeyPress(translate(e.keysym.sym), e.type);
    return true;
}

auto gui_input_listener::keyReleased(const KeyboardEvent& e) -> bool
{
    native_manager().injectKeyRelease(translate(e.keysym.sym));
    return true;
}

auto gui_input_listener::mouseMoved(const MouseMotionEvent& e) -> bool
{
    native_manager().injectMouseMove(e.x, e.y, 0); // 2D
    return true;
}

auto gui_input_listener::mousePressed(const MouseButtonEvent& e) -> bool
{
    native_manager().injectMousePress(e.x, e.y, translate(e.button));
    return true;
}

auto gui_input_listener::mouseReleased(const MouseButtonEvent& e) -> bool
{
    native_manager().injectMouseRelease(e.x, e.y, translate(e.button));
    return true;
}

} // namespace input