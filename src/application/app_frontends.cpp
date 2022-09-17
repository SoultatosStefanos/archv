#include "app.hpp"
#include "commands.hpp"
#include "ui/ui_component.hpp"

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
    setup_gui_overlay();
    setup_gui_tray_manager();
    install_gui_plugins();
    load_gui_resources();
    create_gui();
    set_gui_defaults();
    start_gui();

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

auto app::setup_gui_overlay() -> void
{
    auto* imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui); // takes ownership

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    m_background_renderer->scene()->addRenderQueueListener(ogre_overlay);
}

auto app::setup_gui_tray_manager() -> void
{
    m_tray_manager = std::make_unique< OgreBites::TrayManager >(
        "TrayManager", getRenderWindow());

    m_tray_manager->showCursor();
    addInputListener(m_tray_manager.get());
}

auto app::install_gui_plugins() -> void
{
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
}

// TODO
auto app::load_gui_resources() -> void
{
}

auto app::create_gui() -> void
{
    m_gui = std::make_unique< gui::gui >(m_gui_config);
}

namespace
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        using rgba_type = std::array< float, 4 >;
        return rgba_type { val.r, val.g, val.b, val.a };
    }

} // namespace

// TODO
auto app::set_gui_defaults() -> void
{
    assert(m_gui);

    m_gui->get_editor().set_undo_enabled([this]()
                                         { return m_commands->can_undo(); });

    m_gui->get_editor().set_redo_enabled([this]()
                                         { return m_commands->can_redo(); });

    /*
        auto& gui_configurator =
       m_gui->get_configurator().get_gui_configurator();
        gui_configurator.set_color_theme(m_gui_config.color_theme.c_str());
        gui_configurator.set_frame_rounding(m_gui_config.frame_rounding);
        gui_configurator.set_frame_bordered(m_gui_config.frame_bordered);
        gui_configurator.set_window_bordered(m_gui_config.window_bordered);
        gui_configurator.set_popup_bordered(m_gui_config.popup_bordered);

        auto& bkg_configurator
            = m_gui->get_configurator().get_background_configurator();
        bkg_configurator.set_ambient_color(
            to_rgba(m_rendering_config.background.ambient_light));
        bkg_configurator.set_diffuse_color(
            to_rgba(m_rendering_config.background.diffuse_light));
            */
}

auto app::start_gui() -> void
{
    assert(m_gui);
    ui::start(*m_gui);
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