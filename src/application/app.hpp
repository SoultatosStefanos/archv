// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APP_HPP
#define APPLICATION_APP_HPP

#include "shortcut_input_listener.hpp"
#include "typedefs.hpp"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Bites/OgreTrays.h>
#include <memory>

/***********************************************************
 * Subsystems                                              *
 ***********************************************************/

#include "architecture/all.hpp"
#include "config/all.hpp"
#include "dependencies/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "multithreading/all.hpp"
#include "rendering/all.hpp"
#include "undo_redo/all.hpp"

/***********************************************************
 * Application                                             *
 ***********************************************************/

namespace application
{

// Initializes and hooks all of the subsystems.
// Kick starts the application.
class app : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
    using base = OgreBites::ApplicationContext;

public:
    app(int argc, const char* argv[]);
    virtual ~app() override = default;

    auto frameStarted(const Ogre::FrameEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

    auto setup() -> void override;
    auto shutdown() -> void override;

    auto go() -> void;

    auto paused() const -> bool { return m_paused; }

protected:
    auto toggle_pause_resume() -> void;
    auto toggle_frame_stats() -> void;

    auto make_id_map() const -> architecture::id_map;
    auto make_dependency_map() const -> architecture::dependency_map;
    auto make_weight_map() const -> weight_map;
    auto make_position_map() const -> position_map;

private:
    auto setup_architecture() -> void;
    auto setup_commands() -> void;
    auto setup_dependencies() -> void;
    auto setup_layout() -> void;
    auto setup_background_rendering() -> void;
    auto setup_graph_rendering() -> void;
    auto setup_gui() -> void;
    auto setup_input() -> void;

    auto setup_gui_overlay() -> void;
    auto setup_gui_tray_manager() -> void;
    auto install_gui_plugins() -> void;
    auto create_gui() -> void;
    auto setup_gui_undo_redo() -> void;
    auto setup_gui_gui_configurator() -> void;
    auto setup_gui_background_configurator() -> void;
    auto setup_gui_graph_configurator() -> void;
    auto start_gui() -> void;

    auto connect_gui_with_command_history() -> void;
    auto connect_gui_with_dependencies() -> void;
    auto connect_gui_with_layout() -> void;

    auto connect_rendering_with_layout() -> void;

    auto shutdown_input() -> void;
    auto shutdown_gui() -> void;
    auto shutdown_graph_rendering() -> void;
    auto shutdown_background_rendering() -> void;
    auto shutdown_layout() -> void;
    auto shutdown_dependencies() -> void;
    auto shutdown_commands() -> void;
    auto shutdown_architecture() -> void;

    auto pause() -> void;
    auto resume() -> void;

    const char* m_graph_path { nullptr };

    bool m_paused { false };

    std::unique_ptr< undo_redo::command_history > m_commands;

    /***********************************************************
     * Configs                                                 *
     ***********************************************************/

    dependencies::config_data m_dependencies_config;
    layout::config_data m_layout_config;
    rendering::config_data m_rendering_config;
    gui::config_data m_gui_config;

    /***********************************************************
     * Architecture                                            *
     ***********************************************************/

    architecture::symbol_table m_symbol_table;
    architecture::graph m_graph;

    /***********************************************************
     * Backends                                                *
     ***********************************************************/

    std::unique_ptr< dependencies::backend > m_dependencies_backend;
    std::unique_ptr< layout_backend > m_layout_backend;

    /***********************************************************
     * Rendering                                               *
     ***********************************************************/

    std::unique_ptr< rendering::background_renderer > m_background_renderer;
    std::unique_ptr< graph_renderer > m_graph_renderer;

    /***********************************************************
     * GUI                                                     *
     ***********************************************************/

    std::unique_ptr< gui::gui > m_gui;
    std::unique_ptr< OgreBites::TrayManager > m_tray_manager;

    /***********************************************************
     * Input                                                   *
     ***********************************************************/

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;
    std::unique_ptr< OgreBites::ImGuiInputListener > m_gui_input_listener;
    std::unique_ptr< shortcut_input_listener > m_shortcut_input_listener;
};

} // namespace application

#endif // APPLICATION_APP_HPP
