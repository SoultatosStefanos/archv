// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APPLICATION_HPP
#define APPLICATION_APPLICATION_HPP

#include "input/allfwd.hpp"       // for the input modules
#include "misc/ogre_bitesfwd.hpp" // for CameraMan, ImGuiInputListener, TrayManager
#include "presentation/allfwd.hpp" // for the presentation modules

#include <OGRE/Bites/OgreApplicationContext.h> // for ApplicationContext
#include <memory>                              // for unique_ptr

namespace application
{

// Initializes and hooks all of the subsystems.
// Kick starts the application.
class application : public OgreBites::ApplicationContext
{
    using base = OgreBites::ApplicationContext;

public:
    application(int argc, const char* argv[]);
    ~application() override;

    auto frameStarted(const Ogre::FrameEvent& e) -> bool override;

    auto setup() -> void override;
    auto shutdown() -> void override;

    auto go() -> void;

private:
    using graph_interface_type = presentation::graph_interface;
    using command_history_type = presentation::command_history;

    using background_renderer_type = presentation::background_renderer;
    using graph_renderer_type = presentation::graph_renderer;
    using graph_collision_checker_type = presentation::graph_collision_checker;
    using minimap_renderer_type = presentation::minimap_renderer;

    using gui_type = presentation::overlay_manager;

    using trays_type = OgreBites::TrayManager;
    using cameraman_type = input::camera_handler;
    using gui_input_handler_type = OgreBites::ImGuiInputListener;
    using hud_input_handler_type = input::hud_handler;
    using quit_handler_type = input::quit_handler;
    using shortcut_input_handler_type = input::shortcut_handler;
    using inspection_handler_type = input::inspection_handler;
    using event_dispatcher_type = input::event_dispatcher;

    auto setup_graph_interface() -> void;
    auto setup_commands() -> void;
    auto setup_rendering() -> void;
    auto setup_gui() -> void;
    auto setup_input() -> void;

    auto shutdown_input() -> void;
    auto shutdown_gui() -> void;
    auto shutdown_rendering() -> void;
    auto shutdown_commands() -> void;
    auto shutdown_graph_interface() -> void;

    auto prepare_weights_editor() -> void;
    auto prepare_layout_editor() -> void;
    auto prepare_scaling_editor() -> void;
    auto prepare_degrees_editor() -> void;
    auto prepare_clustering_editor() -> void;
    auto prepare_color_coding_editor() -> void;
    auto prepare_background_configurator() -> void;
    auto prepare_graph_configurator() -> void;
    auto prepare_minimap_configurator() -> void;
    auto prepare_gui_configurator() -> void;
    auto prepare_menu_bar() -> void;

    auto connect_weights_presentation() -> void;
    auto connect_layout_presentation() -> void;
    auto connect_scaling_presentation() -> void;
    auto connect_degrees_presentation() -> void;
    auto connect_clustering_presentation() -> void;
    auto connect_color_coding_presentation() -> void;
    auto connect_background_presentation() -> void;
    auto connect_graph_presentation() -> void;
    auto connect_minimap_presentation() -> void;
    auto connect_gui_presentation() -> void;
    auto connect_menu_bar_presentation() -> void;

    const char* m_graph_path { nullptr };
    const char* m_config_path { nullptr };

    std::unique_ptr< graph_interface_type > m_graph_iface;
    std::unique_ptr< command_history_type > m_cmds;

    std::unique_ptr< graph_renderer_type > m_graph_renderer;
    std::unique_ptr< graph_collision_checker_type > m_graph_collisions;
    std::unique_ptr< background_renderer_type > m_background_renderer;
    std::unique_ptr< minimap_renderer_type > m_minimap_renderer;

    std::unique_ptr< gui_type > m_gui;

    std::unique_ptr< trays_type > m_trays;
    std::unique_ptr< cameraman_type > m_cameraman;
    std::unique_ptr< gui_input_handler_type > m_gui_input_handler;
    std::unique_ptr< hud_input_handler_type > m_hud_input_handler;
    std::unique_ptr< quit_handler_type > m_quit_handler;
    std::unique_ptr< shortcut_input_handler_type > m_shortcut_input_handler;
    std::unique_ptr< inspection_handler_type > m_inspection_input_handler;
    std::unique_ptr< event_dispatcher_type > m_input_chain;
};

} // namespace application

#endif // APPLICATION_APPLICATION_HPP
