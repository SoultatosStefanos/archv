// Contains the paused application state.
// Soultatos Stefanos 2022

#ifndef APPLICATION_PAUSED_STATE_HPP
#define APPLICATION_PAUSED_STATE_HPP

#include "gui/allfwd.hpp"
#include "state.hpp"

#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <memory>

namespace application
{

class state_machine;

class paused_state : public state
{
public:
    using overlay_manager = gui::overlay_manager;
    using pause_menu = gui::pause_menu;

    explicit paused_state(
        state_machine& sm,
        overlay_manager& overlays,
        std::unique_ptr< pause_menu > menu);

    virtual ~paused_state() override = default;

    auto get_gui() const -> const pause_menu& { return *m_gui; }
    auto get_gui() -> pause_menu& { return *m_gui; }

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    auto frameStarted(const Ogre::FrameEvent& e) -> bool override;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto textInput(const OgreBites::TextInputEvent& e) -> bool override;
    auto buttonPressed(const OgreBites::ButtonEvent& e) -> bool override;
    auto buttonReleased(const OgreBites::ButtonEvent& e) -> bool override;

private:
    void handle_undo_combination();
    void handle_redo_combination();

    state_machine& m_sm;
    overlay_manager& m_overlays;

    std::unique_ptr< OgreBites::ImGuiInputListener > m_imgui_input;

    std::unique_ptr< pause_menu > m_gui;

    bool ctrl_pressed { false }, undo_pressed { false }, redo_pressed { false };
};

} // namespace application

#endif // APPLICATION_PAUSED_STATE_HPP
