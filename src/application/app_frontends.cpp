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
    setup_gui_undo_redo();
    setup_gui_background_configurator();
    setup_gui_graph_configurator();
    setup_gui_gui_configurator();
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
    gui::resources::load_materials(
        { "materials/skybox", "materials/edge" }); // FIXME

    gui::resources::load_fonts({ "Fornire-Light" });

    gui::resources::load_meshes({ "cube.mesh" });
}

auto app::create_gui() -> void
{
    m_gui = std::make_unique< gui::gui >(m_gui_config);
}

auto app::setup_gui_undo_redo() -> void
{
    assert(m_gui);

    m_gui->get_editor().set_undo_enabled([this]()
                                         { return m_commands->can_undo(); });

    m_gui->get_editor().set_redo_enabled([this]()
                                         { return m_commands->can_redo(); });
}

namespace
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        using rgba_type = std::array< float, 4 >;
        return rgba_type { val.r, val.g, val.b, val.a };
    }

    inline auto to_scale(const Ogre::Vector3& vec) // FIXME
    {
        return vec.x;
    }

} // namespace

auto app::setup_gui_background_configurator() -> void
{
    assert(m_gui);

    auto& bkg_gui = m_gui->get_configurator().get_background_configurator();
    const auto& cfg = m_rendering_config.background; // FIXME

    bkg_gui.set_skybox_material(cfg.skybox_material);
    bkg_gui.set_skybox_distance(cfg.skybox_distance);
    bkg_gui.set_ambient_color(to_rgba(cfg.ambient_light));
    bkg_gui.set_diffuse_color(to_rgba(cfg.diffuse_light));
    bkg_gui.set_specular_color(to_rgba(cfg.specular_light));
    bkg_gui.set_cam_far_clip_distance(cfg.far_clip_distance);
    bkg_gui.set_cam_near_clip_distance(cfg.near_clip_distance);
}

auto app::setup_gui_graph_configurator() -> void
{
    assert(m_gui);

    auto& graph_gui = m_gui->get_configurator().get_graph_configurator();
    const auto& cfg = m_rendering_config.graph; // FIXME

    graph_gui.set_node_mesh(cfg.vertex_mesh);
    graph_gui.set_node_scale(to_scale(cfg.vertex_scale));
    graph_gui.set_node_font(cfg.vbillboard_font_name);
    graph_gui.set_node_font_color(to_rgba(cfg.vbillboard_color));
    graph_gui.set_node_char_height(cfg.vbillboard_char_height);
    graph_gui.set_node_space_width(cfg.vbillboard_space_width);
    graph_gui.set_edge_material(cfg.edge_material);
}

auto app::setup_gui_gui_configurator() -> void
{
    assert(m_gui);

    auto& gui_configurator = m_gui->get_configurator().get_gui_configurator();
    const auto& cfg = m_gui->config_data();

    gui_configurator.set_color_theme(cfg.color_theme);
    gui_configurator.set_frame_rounding(cfg.frame_rounding);
    gui_configurator.set_frame_bordered(cfg.frame_bordered);
    gui_configurator.set_window_bordered(cfg.window_bordered);
    gui_configurator.set_popup_bordered(cfg.popup_bordered);
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