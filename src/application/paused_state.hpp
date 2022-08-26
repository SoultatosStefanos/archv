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
    using menu_window = gui::menu_window;
    using menu_bar = gui::menu_bar;

    explicit paused_state(
        state_machine& sm,
        overlay_manager& overlays,
        std::unique_ptr< menu_window > win,
        std::unique_ptr< menu_bar > bar);

    virtual ~paused_state() override = default;

    auto get_menu_window() const -> const menu_window& { return *m_win; }
    auto get_menu_window() -> menu_window& { return *m_win; }

    auto get_menu_bar() const -> const menu_bar& { return *m_bar; }
    auto get_menu_bar() -> menu_bar& { return *m_bar; }

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

    std::unique_ptr< menu_window > m_win;
    std::unique_ptr< menu_bar > m_bar;

    bool ctrl_pressed { false }, undo_pressed { false }, redo_pressed { false };
};

} // namespace application

#endif // APPLICATION_PAUSED_STATE_HPP
