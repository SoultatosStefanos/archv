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
    assert(m_gui);
    assert(m_dependencies_backend);

    auto& editor = m_gui->get_editor().get_dependencies_editor();

    editor.connect_to_dependency(
        [this](auto dependency, auto weight)
        {
            BOOST_LOG_TRIVIAL(info) << "assigned " << weight
                                    << " to: " << dependency << " dependency";

            update_dependency_weight(
                *m_commands,
                *m_dependencies_backend,
                std::string(dependency),
                weight);
        });

    editor.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected dependencies restore";

            restore_dependencies_defaults(*m_commands, *m_dependencies_backend);
        });

    m_dependencies_backend->connect(
        [this, &editor](const auto& dependency, auto weight)
        { editor.set_dependency(dependency, weight); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

auto app::connect_gui_with_layout() -> void
{
    assert(m_gui);
    assert(m_layout_backend);

    auto& editor = m_gui->get_editor().get_layout_editor();

    editor.connect_to_layout(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " layout";

            update_layout(
                *m_commands, *m_layout_backend, std::string(selection));
        });

    editor.connect_to_topology(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " topology";

            update_layout_topology(
                *m_commands, *m_layout_backend, std::string(selection));
        });

    editor.connect_to_scale(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " scale";

            layout::update_scale(*m_layout_backend, selection);
            // update_layout_scale(*m_commands, *m_layout_backend, selection);
        });

    editor.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout restore";

            restore_layout_defaults(*m_commands, *m_layout_backend);
        });

    m_layout_backend->connect_to_layout(
        [this, &editor](const auto& layout)
        { editor.set_layout(layout::identify(layout)); });

    m_layout_backend->connect_to_topology(
        [this, &editor](const auto& topology)
        {
            editor.set_topology(layout::identify(topology));
            editor.set_scale(topology.scale());
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with layout management";
}

auto app::connect_gui_with_command_history() -> void
{
    assert(m_gui);

    auto& editor = m_gui->get_editor();

    editor.connect_to_undo([this]() { m_commands->undo(); });
    editor.connect_to_redo([this]() { m_commands->redo(); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
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