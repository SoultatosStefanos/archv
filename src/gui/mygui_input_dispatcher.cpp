#include "mygui_input_dispatcher.hpp"

#include <MYGUI/MyGUI.h>

namespace gui
{

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

void mygui_input_dispatcher::handle(const OgreBites::KeyboardEvent& e) const
{
    native_manager().injectKeyPress(translate(e.keysym.sym), e.type);
}

void mygui_input_dispatcher::handle(const OgreBites::KeyboardEvent& e) const
{
    native_manager().injectKeyRelease(translate(e.keysym.sym));
}

void mygui_input_dispatcher::handle(const OgreBites::MouseMotionEvent& e) const
{
    native_manager().injectMouseMove(e.x, e.y, 0); // 2D
}

void mygui_input_dispatcher::handle(const OgreBites::MouseButtonEvent& e) const
{
    native_manager().injectMousePress(e.x, e.y, translate(e.button));
}

void mygui_input_dispatcher::handle(const OgreBites::MouseButtonEvent& e) const
{
    native_manager().injectMouseRelease(e.x, e.y, translate(e.button));
}

} // namespace gui