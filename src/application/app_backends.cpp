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

/***********************************************************
 * Property Maps                                           *
 ***********************************************************/

auto app::make_id_map() const -> architecture::id_map
{
    return boost::get(boost::vertex_bundle, std::as_const(m_graph));
}

auto app::make_dependency_map() const -> architecture::dependency_map
{
    return boost::get(boost::edge_bundle, std::as_const(m_graph));
}

auto app::make_weight_map() const -> weight_map
{
    assert(m_dependencies_backend);

    return dependencies::make_weight_map< architecture::graph >(
        *m_dependencies_backend, make_dependency_map());
}

auto app::make_position_map() const -> position_map
{
    assert(m_layout_backend);

    return layout::make_position_map(*m_layout_backend);
}

auto app::make_scale_map() const -> scale_map
{
    assert(m_scaling_backend);

    return scaling::make_scale_map< architecture::graph >(
        *m_scaling_backend,
        architecture::metadata_counter(m_symbol_table, m_graph));
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto app::setup_architecture() -> void
{
    assert(m_graph_path);

    const auto& root = config::archive::get().at(m_graph_path);

    std::tie(m_symbol_table, m_graph, std::ignore)
        = architecture::deserialize(root);

    BOOST_LOG_TRIVIAL(info) << "setup architecture";
}

auto app::setup_commands() -> void
{
    m_commands = std::make_unique< undo_redo::command_history >();

    BOOST_LOG_TRIVIAL(info) << "setup commands";
}

auto app::setup_dependencies() -> void
{
    m_dependencies_backend
        = std::make_unique< dependencies::backend >(m_dependencies_config);

    BOOST_LOG_TRIVIAL(info) << "setup dependencies";
}

auto app::setup_layout() -> void
{
    assert(m_dependencies_backend);

    m_layout_backend = std::make_unique< layout_backend >(
        m_graph, make_weight_map(), m_layout_config);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

auto app::setup_scaling() -> void
{
    m_scaling_backend = std::make_unique< scaling::backend >(m_scaling_config);

    BOOST_LOG_TRIVIAL(info) << "setup scaling";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown_scaling() -> void
{
    m_scaling_backend.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown scaling";
}

auto app::shutdown_layout() -> void
{
    m_layout_backend.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown layout";
}

auto app::shutdown_dependencies() -> void
{
    m_dependencies_backend.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown dependencies";
}

auto app::shutdown_commands() -> void
{
    m_commands.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown commands";
}

auto app::shutdown_architecture() -> void
{
    BOOST_LOG_TRIVIAL(info) << "shutdown architecture";
}

} // namespace application