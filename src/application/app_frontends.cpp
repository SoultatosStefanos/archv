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
 * Setup                                                   *
 ***********************************************************/

auto app::setup_background_rendering() -> void
{
    assert(getRenderWindow());

    m_background_renderer = std::make_unique< rendering::background_renderer >(
        *getRenderWindow(), m_rendering_config.background);

    BOOST_LOG_TRIVIAL(info) << "setup background rendering";
}

auto app::setup_graph_rendering() -> void
{
    assert(m_background_renderer);

    m_graph_renderer = std::make_unique< graph_renderer >(
        m_graph,
        make_id_map(),
        make_weight_map(),
        make_position_map(),
        m_background_renderer->scene(),
        m_rendering_config.graph);

    BOOST_LOG_TRIVIAL(info) << "setup graph rendering";
}

auto app::setup_gui() -> void
{
    auto* imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui); // takes ownership

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    m_background_renderer->scene()->addRenderQueueListener(ogre_overlay);

    gui::plugins::install_dependencies(
        [this]()
        {
            auto dependencies = gui::plugins::dependency_map();

            for (const auto& [d, w] : m_dependencies_config)
                dependencies[d.c_str()] = w;

            return dependencies;
        }());

    gui::plugins::install_layouts(
        { std::begin(layout::layout_ids), std::end(layout::layout_ids) });

    gui::plugins::install_topologies(
        { std::begin(layout::topology_ids), std::end(layout::topology_ids) });

    m_gui = std::make_unique< gui::gui >(m_gui_config);

    m_gui->get_editor().set_undo_enabled([this]()
                                         { return m_commands->can_undo(); });

    m_gui->get_editor().set_redo_enabled([this]()
                                         { return m_commands->can_redo(); });

    m_tray_manager = std::make_unique< OgreBites::TrayManager >(
        "TrayManager", getRenderWindow());

    m_tray_manager->showCursor();
    addInputListener(m_tray_manager.get());

    // TODO Defaults, Resources, Plugins

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown_gui() -> void
{
    removeInputListener(m_tray_manager.get());
    m_tray_manager.reset();

    m_gui.reset();

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    m_background_renderer->scene()->removeRenderQueueListener(ogre_overlay);

    // named by Ogre
    Ogre::OverlayManager::getSingleton().destroy("ImGuiOverlay");

    BOOST_LOG_TRIVIAL(info) << "shutdown gui";
}

auto app::shutdown_graph_rendering() -> void
{
    m_graph_renderer.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown graph rendering";
}

auto app::shutdown_background_rendering() -> void
{
    m_background_renderer.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown background rendering";
}

} // namespace application