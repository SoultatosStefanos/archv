#include "app.hpp"
#include "commands.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>

namespace application
{

auto app::connect_gui_with_dependencies() -> void
{

    // assert(m_menu_window);
    // assert(m_commands);
    // assert(m_dependencies_backend);

    // m_menu_window->connect_to_dependency(
    //     [this](const auto& dependency, auto weight)
    //     {
    //         update_dependency_weight(
    //             *m_commands, *m_dependencies_backend, dependency, weight);
    //     });

    // m_menu_window->connect_to_dependencies_restore(
    //     [this]() {
    //         restore_dependencies_defaults(*m_commands,
    //         *m_dependencies_backend);
    //     });

    // m_dependencies_backend->connect(
    //     [this](const auto& dependency, auto weight)
    //     { m_menu_window->set_dependency(dependency, weight); });

    // BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

auto app::connect_gui_with_layout() -> void
{
    // assert(m_menu_window);
    // assert(m_layout_backend);

    // m_menu_window->connect_to_layout(
    //     [this](const auto& selection)
    //     { update_layout(*m_commands, *m_layout_backend, selection); });

    // m_menu_window->connect_to_topology(
    //     [this](const auto& selection)
    //     { update_layout_topology(*m_commands, *m_layout_backend, selection);
    //     });

    // m_menu_window->connect_to_scale(
    //     [this](auto selection)
    //     {
    //         layout::update_scale(*m_layout_backend, selection);
    //         // update_layout_scale(*m_commands, *m_layout_backend,
    //         selection);
    //     });

    // m_menu_window->connect_to_layout_restore(
    //     [this]() { restore_layout_defaults(*m_commands, *m_layout_backend);
    //     });

    // m_layout_backend->connect_to_layout(
    //     [this](const auto& layout)
    //     { m_menu_window->set_layout(layout::identify(layout)); });

    // m_layout_backend->connect_to_topology(
    //     [this](const auto& topology)
    //     {
    //         m_menu_window->set_topology(layout::identify(topology));
    //         m_menu_window->set_scale(topology.scale());
    //     });

    // BOOST_LOG_TRIVIAL(info) << "connected gui with layout management";
}

auto app::connect_gui_with_command_history() -> void
{
    // assert(m_menu_bar);
    // assert(m_commands);

    // m_menu_bar->connect_to_undo([this]() { m_commands->undo(); });
    // m_menu_bar->connect_to_redo([this]() { m_commands->redo(); });

    // BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
}

auto app::connect_rendering_with_layout() -> void
{
    assert(m_graph_renderer);
    assert(m_layout_backend);

    m_layout_backend->connect_to_layout(
        [this](const auto&)
        { m_graph_renderer->set_vertex_pos(make_position_map()); });
}

} // namespace application