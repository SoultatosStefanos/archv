// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APP_HPP
#define APPLICATION_APP_HPP

#include "config.hpp"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Bites/OgreInput.h>
#include <memory>

/***********************************************************
 * Subsystems                                              *
 ***********************************************************/

#include "architecture/all.hpp"
#include "dependencies/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "multithreading/all.hpp"
#include "rendering/all.hpp"
#include "undo_redo/all.hpp"

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
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

    auto setup() -> void override;
    auto shutdown() -> void override;

    auto go() -> void;

protected:
    using weight_map = dependencies::
        dynamic_weight_map< architecture::graph, architecture::dependency_map >;

    using position_map
        = layout::dynamic_position_map< architecture::graph, weight_map >;

    auto config() const -> const auto& { return m_config; }
    auto config() -> auto& { return m_config; }

    auto dependencies_config() const -> const auto& { return m_deps_config; }
    auto dependencies_config() -> auto& { return m_deps_config; }

    auto layout_config() const -> const auto& { return m_layout_config; }
    auto layout_config() -> auto& { return m_layout_config; }

    auto symbol_table() const -> const auto& { return m_st; }
    auto symbol_table() -> auto& { return m_st; }

    auto graph() const -> const auto& { return m_g; }
    auto graph() -> auto& { return m_g; }

    auto cmd_history() const -> auto* { return m_cmds.get(); }

    auto dependencies_core() const -> auto* { return m_deps.get(); }
    auto layout_core() const -> auto* { return m_layout.get(); }

    auto background_renderer() const -> auto* { return m_bkgrd_renderer.get(); }
    auto graph_renderer() const -> auto* { return m_g_renderer.get(); }
    auto cameraman() const -> auto* { return m_cameraman.get(); }
    auto gui_input_handler() const -> auto* { return m_gui_input.get(); }

    auto menu_window() const -> auto* { return m_menu_win.get(); }
    auto menu_bar() const -> auto* { return m_menu_bar.get(); }

    auto paused() const -> bool { return m_paused; }

    auto toggle_pause_resume() -> void
    {
        if (!paused())
            pause();
        else
            resume();
    }

    auto id_map() const -> architecture::id_map;
    auto dependency_map() const -> architecture::dependency_map;
    auto dynamic_weight_map() const -> weight_map;
    auto dynamic_position_map() const -> position_map;

private:
    auto setup_architecture() -> void;
    auto setup_commands() -> void;
    auto setup_dependencies() -> void;
    auto setup_layout() -> void;
    auto setup_rendering() -> void;
    auto setup_gui() -> void;
    auto setup_input() -> void;

    auto connect_gui_with_dependencies() -> void;
    auto connect_gui_with_layout() -> void;
    auto connect_gui_with_command_history() -> void;

    auto shutdown_input() -> void;
    auto shutdown_gui() -> void;
    auto shutdown_rendering() -> void;
    auto shutdown_layout() -> void;
    auto shutdown_dependencies() -> void;
    auto shutdown_commands() -> void;
    auto shutdown_architecture() -> void;

    auto pause() -> void;
    auto resume() -> void;

    bool m_paused { false };

    config_data m_config;
    dependencies::config_data m_deps_config;
    layout::config_data m_layout_config;

    architecture::symbol_table m_st;
    architecture::graph m_g;

    std::unique_ptr< undo_redo::command_history > m_cmds;
    std::unique_ptr< dependencies::core > m_deps;
    std::unique_ptr< layout::core< architecture::graph, weight_map > > m_layout;

    std::unique_ptr< rendering::background_renderer > m_bkgrd_renderer;

    std::unique_ptr< rendering::graph_renderer<
        architecture::graph,
        architecture::id_map,
        weight_map,
        position_map > >
        m_g_renderer;

    std::unique_ptr< gui::menu_window > m_menu_win;
    std::unique_ptr< gui::menu_bar > m_menu_bar;

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;
    std::unique_ptr< OgreBites::ImGuiInputListener > m_gui_input;
};

} // namespace application

#endif // APPLICATION_APP_HPP
