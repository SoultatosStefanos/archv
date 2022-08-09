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

// TODO
class pause_menu : public view::state
{
public:
    using state_machine = view::state_machine;

    using dependencies = detail::pause_menu_gui::dependencies;
    using layout_options = detail::pause_menu_gui::layout_options;
    using topology_options = detail::pause_menu_gui::topology_options;
    using scale_options = detail::pause_menu_gui::scale_options;

    using dependency_slot = detail::pause_menu_gui::dependency_slot;
    using layout_slot = detail::pause_menu_gui::layout_slot;
    using topology_slot = detail::pause_menu_gui::topology_slot;
    using scale_slot = detail::pause_menu_gui::scale_slot;
    using connection = detail::pause_menu_gui::connection;

    explicit pause_menu(
        state_machine& sm,
        dependencies deps = dependencies(),
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    virtual ~pause_menu() override = default;

    auto connect_to_dependency(const dependency_slot& slot) -> connection
    {
        return m_gui.connect_to_dependency(slot);
    }

    auto connect_to_layout(const layout_slot& slot) -> connection
    {
        return m_gui.connect_to_layout(slot);
    }

    auto connect_to_topology(const topology_slot& slot) -> connection
    {
        return m_gui.connect_to_topology(slot);
    }

    auto connect_to_scale(const scale_slot& slot) -> connection
    {
        return m_gui.connect_to_scale(slot);
    }

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
    state_machine& m_sm;

    Ogre::SceneManager* m_scene; // From global context

    std::unique_ptr< OgreBites::ImGuiInputListener > m_imgui_input;

    detail::pause_menu_gui m_gui;
};

} // namespace gui

#endif // GUI_PAUSE_MENU_HPP
