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
#include "layout/all.hpp"
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
    using graph = architecture::graph;
    using dependency_map = architecture::dependency_map;
    using weight_map
        = dependencies::dynamic_weight_map< graph, dependency_map >;

    using view_manager = view::core;
    using command_manager = undo_redo::command_history;
    using arch_manager = architecture::core;
    using deps_manager = dependencies::core;
    using layout_manager = layout::core< graph, weight_map >;

    auto get_view_manager() const -> const view_manager&;
    auto get_view_manager() -> view_manager&;

    auto get_cmd_manager() const -> const command_manager&;
    auto get_cmd_manager() -> command_manager&;

    auto get_arch_manager() const -> const arch_manager&;
    auto get_arch_manager() -> arch_manager&;

    auto get_deps_manager() const -> const deps_manager&;
    auto get_deps_manager() -> deps_manager&;

    auto get_layout_manager() const -> const layout_manager&;
    auto get_layout_manager() -> layout_manager&;

    void draw_layout(const layout::layout< graph >& l);

private:
    void setup_architecture();
    void setup_commands();
    void setup_dependencies();
    void setup_layout();
    void setup_view();

    void shutdown_view();
    void shutdown_layout();
    void shutdown_dependencies();
    void shutdown_commands();
    void shutdown_architecture();

    std::unique_ptr< view_manager > m_view_manager;
    std::unique_ptr< command_manager > m_cmd_manager;
    std::unique_ptr< arch_manager > m_arch_manager;
    std::unique_ptr< deps_manager > m_deps_manager;
    std::unique_ptr< layout_manager > m_layout_manager;
};

#endif // APP_HPP
