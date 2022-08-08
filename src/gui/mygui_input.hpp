// Contains a utility module for delegating input events to the MYGUI input
// handler.
// Soultatos Stefanos 2022

#ifndef GUI_MYGUI_INPUT_HPP
#define GUI_MYGUI_INPUT_HPP

#include <OGRE/Bites/OgreInput.h>

namespace gui
{

void mygui_dispatch(const OgreBites::KeyboardEvent& e);
void mygui_dispatch(const OgreBites::KeyboardEvent& e);
void mygui_dispatch(const OgreBites::MouseMotionEvent& e);
void mygui_dispatch(const OgreBites::MouseButtonEvent& e);
void mygui_dispatch(const OgreBites::MouseButtonEvent& e);

} // namespace gui

#endif // GUI_MYGUI_INPUT_HPP
