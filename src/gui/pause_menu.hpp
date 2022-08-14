// Contains the pause menu gui of the application.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_HPP
#define GUI_PAUSE_MENU_HPP

#include "pause_menu_private.hpp"
#include "view/state.hpp"

#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreSceneManager.h>
#include <memory>

namespace view
{
class state_machine;
}

namespace gui
{

// Facade class for the pause menu front end.
class pause_menu : public view::state
{
public:
    using state_machine = view::state_machine;

    using dependency_options = detail::pause_menu_window::dependency_options;
    using layout_options = detail::pause_menu_window::layout_options;
    using topology_options = detail::pause_menu_window::topology_options;
    using scale_options = detail::pause_menu_window::scale_options;

    using undo_enabled = detail::pause_menu_bar::undo_enabled;
    using redo_enabled = detail::pause_menu_bar::redo_enabled;

    using dependency_slot = detail::pause_menu_window::dependency_slot;
    using layout_slot = detail::pause_menu_window::layout_slot;
    using topology_slot = detail::pause_menu_window::topology_slot;
    using scale_slot = detail::pause_menu_window::scale_slot;
    using dependencies_restore_slot
        = detail::pause_menu_window::dependencies_restore_slot;
    using layout_restore_slot = detail::pause_menu_window::layout_restore_slot;
    using undo_slot = detail::pause_menu_bar::undo_slot;
    using redo_slot = detail::pause_menu_bar::redo_slot;
    using connection = detail::pause_menu_window::connection;

    explicit pause_menu(
        state_machine& sm,
        dependency_options dependencies = dependency_options(),
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    virtual ~pause_menu() override = default;

    void set_undo_enabled(undo_enabled pred)
    {
        m_bar.set_undo_enabled(std::move(pred));
    }

    void set_redo_enabled(redo_enabled pred)
    {
        m_bar.set_redo_enabled(std::move(pred));
    }

    auto connect_to_dependency(const dependency_slot& slot) -> connection
    {
        return m_win.connect_to_dependency(slot);
    }

    auto connect_to_layout(const layout_slot& slot) -> connection
    {
        return m_win.connect_to_layout(slot);
    }

    auto connect_to_topology(const topology_slot& slot) -> connection
    {
        return m_win.connect_to_topology(slot);
    }

    auto connect_to_scale(const scale_slot& slot) -> connection
    {
        return m_win.connect_to_scale(slot);
    }

    auto connect_to_undo(const undo_slot& f) -> connection
    {
        return m_bar.connect_to_undo(f);
    }

    auto connect_to_redo(const redo_slot& f) -> connection
    {
        return m_bar.connect_to_redo(f);
    }

    auto connect_to_dependencies_restore(const dependencies_restore_slot& slot)
        -> connection
    {
        return m_win.connect_to_dependencies_restore(slot);
    }

    auto connect_to_layout_restore(const layout_restore_slot& slot)
        -> connection
    {
        return m_win.connect_to_layout_restore(slot);
    }

    void set_dependency(const std::string& type, int weight)
    {
        m_win.set_dependency(type, weight);
    }

    void set_layout(const std::string& type) { m_win.set_layout(type); }
    void set_topology(const std::string& type) { m_win.set_topology(type); }
    void set_scale(double val) { m_win.set_scale(val); }

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
    auto scene() const -> Ogre::SceneManager*;

    void handle_undo_combination();
    void handle_redo_combination();

    state_machine& m_sm;

    std::unique_ptr< OgreBites::ImGuiInputListener > m_imgui_input;

    detail::pause_menu_window m_win;
    detail::pause_menu_bar m_bar;

    bool ctrl_pressed { false }, undo_pressed { false }, redo_pressed { false };
};

} // namespace gui

#endif // GUI_PAUSE_MENU_HPP
