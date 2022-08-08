// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <memory>

/***********************************************************
 * Subsystems                                              *
 ***********************************************************/

#include "architecture/all.hpp"
#include "config/all.hpp"
#include "dependencies/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "rendering/all.hpp"
#include "undo_redo/all.hpp"
#include "view/all.hpp"

/***********************************************************
 * Application                                             *
 ***********************************************************/

// Initializes and hooks all of the subsystems.
// Kick starts the application.
class app : public OgreBites::ApplicationContext
{
    using base = OgreBites::ApplicationContext;

public:
    explicit app(const std::string& name = "ARCHV");
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

    void go();

protected:
    using symbol_table = architecture::symbol_table;

    using graph = architecture::graph;
    using dependency_map = architecture::dependency_map;
    using weight_map
        = dependencies::dynamic_weight_map< graph, dependency_map >;

    using command_history = undo_redo::command_history;
    using dependencies_core = dependencies::core;
    using layout_core = layout::core< graph, weight_map >;

    using state_machine = view::state_machine;
    using state_input_dispatcher = view::state_input_dispatcher;
    using graph_visualization = rendering::graph_visualization;
    using pause_menu = gui::pause_menu;

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

    auto get_graph_visualization() const -> const graph_visualization&;
    auto get_graph_visualization() -> graph_visualization&;

    auto get_pause_menu() const -> const pause_menu&;
    auto get_pause_menu() -> pause_menu&;

    void lay_graph(const layout::layout< graph >& l);

private:
    void setup_architecture();
    void setup_commands();
    void setup_dependencies();
    void setup_layout();
    void setup_view();
    void setup_gui();
    void setup_rendering();

    void shutdown_rendering();
    void shutdown_gui();
    void shutdown_view();
    void shutdown_layout();
    void shutdown_dependencies();
    void shutdown_commands();
    void shutdown_architecture();

    symbol_table m_st;
    graph m_g;

    std::unique_ptr< command_history > m_cmds;
    std::unique_ptr< dependencies_core > m_dependencies;
    std::unique_ptr< layout_core > m_layout;

    std::unique_ptr< state_machine > m_sm;
    std::unique_ptr< state_input_dispatcher > m_input;
    std::unique_ptr< graph_visualization > m_graph_visualization;
    std::unique_ptr< pause_menu > m_pause_menu;
};

#endif // APP_HPP
