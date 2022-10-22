#include "application.hpp"

#include "archive.hpp"
#include "commands.hpp"
#include "config.hpp"

#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>
#include <ranges>

namespace application
{

app::app(int argc, const char* argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: `./<exec> <path/to/graph.json>`\n";
        std::exit(EXIT_FAILURE);
    }

    m_graph_path = argv[1];
    const auto& jsons = archive::get();

    m_weights_config
        = weights::deserialize(jsons.at(ARCHV_WEIGHTS_CONFIG_PATH));

    m_layout_config = layout::deserialize(jsons.at(ARCHV_LAYOUT_CONFIG_PATH));

    m_scaling_config
        = scaling::deserialize(jsons.at(ARCHV_SCALING_CONFIG_PATH));

    m_rendering_config
        = rendering::deserialize(jsons.at(ARCHV_RENDERING_CONFIG_PATH));

    m_gui_config = gui::deserialize(jsons.at(ARCHV_GUI_CONFIG_PATH));
}

auto app::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    base::frameStarted(e);
    Ogre::ImGuiOverlay::NewFrame();
    if (m_pause_resume_handler->paused())
        m_gui->render();
    return true;
}

auto app::setup() -> void
{
    setup_graph_interface();
    setup_commands();
    setup_background_renderer();
    setup_graph_renderer();
    setup_gui();
    setup_input();

    prepare_weights_editor();
    prepare_layout_editor();
    prepare_scaling_editor();
    prepare_background_configurator();
    prepare_graph_configurator();
    prepare_gui_configurator();
    prepare_gui_undo_redo();

    connect_weights_presentation();
    connect_layout_presentation();
    connect_scaling_presentation();
    connect_background_presentation();
    connect_graph_presentation();
    connect_gui_presentation();
    connect_undo_redo_presentation();
}

auto app::shutdown() -> void
{
    shutdown_input();
    shutdown_gui();
    shutdown_graph_renderer();
    shutdown_background_renderer();
    shutdown_commands();
    shutdown_graph_interface();
}

auto app::go() -> void
{
    getRoot()->startRendering();
}

auto app::setup_graph_interface() -> void
{
    const auto& jsons = archive::get();
    auto&& [st, g, _] = architecture::deserialize(jsons.at(m_graph_path));

    m_graph_iface = std::make_unique< graph_interface_type >(
        std::move(st),
        std::move(g),
        m_weights_config,
        m_layout_config,
        m_scaling_config);
}

auto app::setup_commands() -> void
{
    m_cmds = std::make_unique< command_history_type >();
}

auto app::setup_background_renderer() -> void
{
    m_background_renderer = std::make_unique< background_renderer_type >(
        *getRenderWindow(),
        m_rendering_config.background,
        ARCHV_RESOURCE_GROUP);
}

auto app::setup_graph_renderer() -> void
{
    m_graph_renderer = std::make_unique< graph_renderer_type >(
        m_graph_iface->get_graph(),
        m_graph_iface->vertex_id(),
        m_graph_iface->vertex_position(),
        m_graph_iface->edge_dependency(),
        m_background_renderer->scene(),
        m_rendering_config.graph,
        ARCHV_RESOURCE_GROUP);

    m_graph_renderer->render_scaling(m_graph_iface->vertex_scale());
    m_graph_renderer->render_weights(m_graph_iface->edge_weight());
}

namespace
{
    template < typename InputRange >
    inline auto to_plugin_set(const InputRange& data)
    {
        auto res = std::unordered_set< std::string_view >();
        std::transform(
            std::cbegin(data),
            std::cend(data),
            std::inserter(res, std::begin(res)),
            [](const auto& str) { return str.c_str(); });
        return res;
    }

    inline auto install_gui_plugins(
        const weights::config_data& wcfg,
        const layout::config_data& lcf,
        const scaling::config_data& scfg)
    {
        using std::ranges::views::keys;
        gui::plugins::install_dependencies(to_plugin_set(keys(wcfg)));
        gui::plugins::install_layouts(to_plugin_set(lcf.layouts));
        gui::plugins::install_topologies(to_plugin_set(lcf.topologies));
        gui::plugins::install_factors(to_plugin_set(keys(scfg)));
    }

    inline auto load_gui_resources()
    {
    }

} // namespace

auto app::setup_gui() -> void
{
}

auto app::setup_input() -> void
{
}

auto app::shutdown_input() -> void
{
}

auto app::shutdown_gui() -> void
{
}

auto app::shutdown_graph_renderer() -> void
{
}

auto app::shutdown_background_renderer() -> void
{
}

auto app::shutdown_commands() -> void
{
}

auto app::shutdown_graph_interface() -> void
{
}

auto app::prepare_weights_editor() -> void
{
}

auto app::prepare_layout_editor() -> void
{
}

auto app::prepare_scaling_editor() -> void
{
}

auto app::prepare_background_configurator() -> void
{
}

auto app::prepare_graph_configurator() -> void
{
}

auto app::prepare_gui_configurator() -> void
{
}

auto app::prepare_gui_undo_redo() -> void
{
}

auto app::connect_weights_presentation() -> void
{
}

auto app::connect_layout_presentation() -> void
{
}

auto app::connect_scaling_presentation() -> void
{
}

auto app::connect_background_presentation() -> void
{
}

auto app::connect_graph_presentation() -> void
{
}

auto app::connect_gui_presentation() -> void
{
}

auto app::connect_undo_redo_presentation() -> void
{
}
} // namespace application