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

template <
    typename PauseMenuGUI,
    typename GUIManager,
    typename GUIInputDispatcher >
class paused_state : public state
{
public:
    using pause_menu_gui = PauseMenuGUI;
    using gui_manager = GUIManager;
    using gui_input_dispatcher = GUIInputDispatcher;

    paused_state(
        pause_menu_gui* pause_menu,
        gui_manager& guis,
        gui_input_dispatcher& input_dispatcher,
        state_machine& machine,
        state* menu_state = nullptr)
    : m_pause_menu { pause_menu }
    , m_guis { guis }
    , m_input_dispatcher { input_dispatcher }
    , m_machine { machine }
    , m_menu_state { menu_state }
    {
    }

    virtual ~paused_state() override
    {
        if (m_pause_menu)
            exit();
    }

    paused_state(const paused_state&) = delete;
    paused_state(paused_state&&) = delete;

    auto operator=(const paused_state&) -> paused_state& = delete;
    auto operator=(paused_state&&) -> paused_state& = delete;

    virtual void enter() override
    {
        m_pause_menu = guis.create_ui< pause_menu_gui >();
    }

    virtual void exit() override { guis.destroy_ui(m_pause_menu); }

    virtual void pause() override { }
    virtual void resume() override { }

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override
    {
        m_input_dispatcher.handle(e);

        // TODO Advance to next state
    }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override
    {
        m_input_dispatcher.handle(e);
    }

    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override
    {
        m_input_dispatcher.handle(e);
    }

    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override
    {
        m_input_dispatcher.handle(e);
    }

    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override
    {
        m_input_dispatcher.handle(e);
    }

private:
    pause_menu_gui* m_pause_menu { nullptr };
    gui_manager& m_guis;
    gui_input_dispatcher& m_input_dispatcher;

    state_machine& m_machine;
    state* m_menu_state { nullptr };
};

} // namespace view

#endif // VIEW_PAUSED_STATE_HPP
