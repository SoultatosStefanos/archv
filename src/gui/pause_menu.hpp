// Contains the pause menu gui of the application.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_HPP
#define GUI_PAUSE_MENU_HPP

#include "view/state.hpp"

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreSceneManager.h>

namespace view
{
class state_machine;
}

namespace gui
{

// TODO
class pause_menu : public view::state
{
public:
    using state_machine = view::state_machine;

    pause_menu(state_machine& sm);
    virtual ~pause_menu() override = default;

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    void frameRendered(const Ogre::FrameEvent& e) override;
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    state_machine& m_sm;

    Ogre::SceneManager* m_scene; // From global context
};

} // namespace gui

#endif // GUI_PAUSE_MENU_HPP
