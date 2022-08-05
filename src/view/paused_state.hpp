// Contains the application paused state that provides the UI.
// Soultatos Stefanos 2022

#ifndef VIEW_PAUSED_STATE_HPP
#define VIEW_PAUSED_STATE_HPP

#include "state.hpp"

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <memory>

namespace view
{

class state_machine;

class paused_state : public state
{
public:
    paused_state(
        Ogre::RenderWindow& window,
        state_machine& machine,
        state* menu_state = nullptr);

    virtual ~paused_state() override = default;

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;

private:
    void setup_scene();
    void setup_gui();

    void shutdown_gui();
    void shutdown_scene();

    Ogre::Root& m_root; // Obtained from global context
    Ogre::RenderWindow& m_window;
    state_machine& m_machine;
    state* m_menu_state { nullptr };

    Ogre::SceneManager* m_scene;
    std::unique_ptr< MyGUI::OgrePlatform > m_platform;
    std::unique_ptr< MyGUI::Gui > m_gui;
};

} // namespace view

#endif // VIEW_PAUSED_STATE_HPP
