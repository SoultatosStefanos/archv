#include "application.hpp"

#include "architecture/all.hpp"
#include "config/all.hpp"
#include "degrees/all.hpp"
#include "dependencies/all.hpp"
#include "gui/all.hpp"
#include "layout/all.hpp"
#include "multithreading/all.hpp"
#include "rendering/all.hpp"
#include "scaling/all.hpp"
#include "undo_redo/all.hpp"

#include <OGRE/OgreRoot.h>
#include <iostream>

namespace application
{

application::application(int argc, const char* argv[]) : base("ARCHV")
{
    if (argc != 2)
    {
        std::cout << "usage: `./<exec> <path/to/graph.json>`\n";
        std::exit(EXIT_FAILURE);
    }

    m_graph_path = argv[1];
}

auto application::setup() -> void
{
    base::setup();
    setup_graph();
    setup_commands();
    setup_background_renderer();
    setup_graph_renderer();
    setup_gui();
    setup_background_presenter();
    setup_graph_presenter();
    setup_commands_presenter();
    setup_gui_presenter();
}

auto application::shutdown() -> void
{
    shutdown_gui_presenter();
    shutdown_commands_presenter();
    shutdown_graph_presenter();
    shutdown_background_presenter();
    shutdown_gui();
    shutdown_graph_renderer();
    shutdown_background_renderer();
    shutdown_commands();
    shutdown_graph();
    base::shutdown();
}

auto application::go() -> void
{
    getRoot()->startRendering();
}

auto application::setup_graph() -> void
{
    const auto& archive = config::archive::get();
    const auto& arch_root = archive.at(get_graph_path());
    const auto& deps_root = archive.at(ARCHV_DEPS_CONFIG_PATH);
    const auto& layout_root = archive.at(ARCHV_LAYOUT_CONFIG_PATH);
    const auto& scale_root = archive.at(ARCHV_SCALING_CONFIG_PATH);

    auto&& deps_cfg = dependencies::deserialize(deps_root);
    auto&& layout_cfg = layout::deserialize(layout_root);
    auto&& scale_cfg = scaling::deserialize(scale_root);
    auto&& [symbol_table, graph, _] = architecture::deserialize(arch_root);

    m_graph_iface = std::make_unique< graph_interface >(
        std::move(symbol_table),
        std::move(graph),
        std::move(deps_cfg),
        std::move(layout_cfg),
        std::move(scale_cfg));
}

auto application::setup_commands() -> void
{
    m_commands = std::make_unique< command_history_type >();
}

auto application::setup_background_renderer() -> void
{
    m_background_renderer = std::make_unique< background_renderer_type >(
        *getRenderWindow(),
        m_rendering_config.background,
        ARCHV_RESOURCE_GROUP);
}

auto application::setup_graph_renderer() -> void
{
}

auto application::setup_gui() -> void
{
}

auto application::setup_background_presenter() -> void
{
}

auto application::setup_graph_presenter() -> void
{
}

auto application::setup_commands_presenter() -> void
{
}

auto application::setup_gui_presenter() -> void
{
}

auto application::shutdown_gui_presenter() -> void
{
}

auto application::shutdown_commands_presenter() -> void
{
}

auto application::shutdown_graph_presenter() -> void
{
}

auto application::shutdown_background_presenter() -> void
{
}

auto application::shutdown_gui() -> void
{
}

auto application::shutdown_graph_renderer() -> void
{
}

auto application::shutdown_background_renderer() -> void
{
}

auto application::shutdown_commands() -> void
{
}

auto application::shutdown_graph() -> void
{
}

} // namespace application