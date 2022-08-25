// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APP_HPP
#define APPLICATION_APP_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <memory>

/***********************************************************
 * Subsystems                                              *
 ***********************************************************/

#include "architecture/all.hpp"
#include "config.hpp"
#include "dependencies/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "paused_state.hpp"
#include "rendering/all.hpp"
#include "state_machine.hpp"
#include "undo_redo/all.hpp"
#include "visualization_state.hpp"

#include "json/all.hpp"

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
    auto frameRenderingQueued(const Ogre::FrameEvent& e) -> bool override;
    auto frameEnded(const Ogre::FrameEvent& e) -> bool override;

    auto frameRendered(const Ogre::FrameEvent& e) -> void override;
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto touchMoved(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchPressed(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchReleased(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto textInput(const OgreBites::TextInputEvent& e) -> bool override;
    auto axisMoved(const OgreBites::AxisEvent& e) -> bool override;
    auto buttonPressed(const OgreBites::ButtonEvent& e) -> bool override;
    auto buttonReleased(const OgreBites::ButtonEvent& e) -> bool override;

    void setup() override;
    void shutdown() override;

    void go();

protected:
    using main_config = config_data;
    using dependencies_config = dependencies::config_data;
    using layout_config = layout::config_data;

    using symbol_table = architecture::symbol_table;

    using graph = architecture::graph;
    using dependency_map = architecture::dependency_map;
    using weight_map
        = dependencies::dynamic_weight_map< graph, dependency_map >;

    using command_history = undo_redo::command_history;
    using dependencies_core = dependencies::core;
    using layout_core = layout::core< graph, weight_map >;

    using overlay_manager = gui::overlay_manager;

    auto get_main_config() const -> const main_config&;
    auto get_main_config() -> main_config&;

    auto get_dependencies_config() const -> const dependencies_config&;
    auto get_dependencies_config() -> dependencies_config&;

    auto get_layout_config() const -> const layout_config&;
    auto get_layout_config() -> layout_config&;

    auto get_symbol_table() const -> const symbol_table&;
    auto get_symbol_table() -> symbol_table&;

    auto get_graph() const -> const graph&;
    auto get_graph() -> graph&;

    auto get_cmds() const -> const command_history&;
    auto get_cmds() -> command_history&;

    auto get_dependencies_core() const -> const dependencies_core&;
    auto get_dependencies_core() -> dependencies_core&;

    auto get_layout_core() const -> const layout_core&;
    auto get_layout_core() -> layout_core&;

    auto get_state_machine() const -> const state_machine&;
    auto get_state_machine() -> state_machine&;

    auto get_overlay_manager() const -> const overlay_manager&;
    auto get_overlay_manager() -> overlay_manager&;

    auto get_visualization_state() const -> const visualization_state&;
    auto get_visualization_state() -> visualization_state&;

    auto get_paused_state() const -> const paused_state&;
    auto get_paused_state() -> paused_state&;

    auto get_io() const -> const auto& { return m_io; }
    auto get_io() -> auto& { return m_io; }

    void lay_graph(const layout::layout< graph >& l);

private:
    void setup_architecture();
    void setup_commands();
    void setup_dependencies();
    void setup_layout();
    void setup_fsm();
    void setup_gui();
    void setup_rendering();

    void connect_gui_with_dependencies();
    void connect_gui_with_layout();
    void connect_layout_with_rendering();
    void connect_gui_with_command_history();

    void shutdown_rendering();
    void shutdown_gui();
    void shutdown_fsm();
    void shutdown_layout();
    void shutdown_dependencies();
    void shutdown_commands();
    void shutdown_architecture();

    main_config m_main_config;
    dependencies_config m_dependencies_config;
    layout_config m_layout_config;

    symbol_table m_st;
    graph m_g;

    std::unique_ptr< command_history > m_cmds;
    std::unique_ptr< dependencies_core > m_dependencies;
    std::unique_ptr< layout_core > m_layout;

    std::unique_ptr< state_machine > m_sm;
    std::unique_ptr< overlay_manager > m_overlays;
    std::unique_ptr< visualization_state > m_visualization;
    std::unique_ptr< paused_state > m_paused;

    boost::asio::io_service m_io;

    // avoid immediate exit
    using executor_type = boost::asio::io_context::executor_type;
    boost::asio::executor_work_guard< executor_type > m_work;
};

} // namespace application

#endif // APPLICATION_APP_HPP
