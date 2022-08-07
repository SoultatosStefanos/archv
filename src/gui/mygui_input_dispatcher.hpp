// Contains a utility class responsible for delegating input events to the
// MYGUI input handling module.
// Soultatos Stefanos 2022

#ifndef GUI_MYGUI_INPUT_DISPATCHER_HPP
#define GUI_MYGUI_INPUT_DISPATCHER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace gui
{

class mygui_input_dispatcher final
{
public:
    mygui_input_dispatcher(const mygui_input_dispatcher&) = delete;
    mygui_input_dispatcher(mygui_input_dispatcher&&) = delete;

    auto operator=(const mygui_input_dispatcher&)
        -> mygui_input_dispatcher& = delete;

    auto operator=(mygui_input_dispatcher&&)
        -> mygui_input_dispatcher& = delete;

    static auto get() -> mygui_input_dispatcher&
    {
        static mygui_input_dispatcher singleton;
        return singleton;
    }

    void handle(const OgreBites::KeyboardEvent& e) const;
    void handle(const OgreBites::KeyboardEvent& e) const;
    void handle(const OgreBites::MouseMotionEvent& e) const;
    void handle(const OgreBites::MouseButtonEvent& e) const;
    void handle(const OgreBites::MouseButtonEvent& e) const;

private:
    mygui_input_dispatcher() = default;
    ~mygui_input_dispatcher() = default;
};

} // namespace gui

#endif // GUI_MYGUI_INPUT_DISPATCHER_HPP
