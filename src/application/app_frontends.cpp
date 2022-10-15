#include "app.hpp"
#include "commands.hpp"
#include "ui/ui_component.hpp"

#include <OGRE/Ogre.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>
#include <ranges>

namespace application
{

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto app::setup_background_rendering() -> void
{
    assert(getRenderWindow());

    m_background_renderer = std::make_unique< rendering::background_renderer >(
        *getRenderWindow(),
        m_rendering_config.background,
        ARCHV_RESOURCE_GROUP);

    BOOST_LOG_TRIVIAL(info) << "setup background rendering";
}

// NOTE: Parses graph multiple times, could be optimized, but kept like this for
// simplicity's sake (Will be parsed multiple times on beggining only).
auto app::setup_graph_rendering() -> void
{
    assert(m_background_renderer);

    m_graph_renderer = std::make_unique< graph_renderer >(
        m_graph,
        make_id_map(),
        make_position_map(),
        make_dependency_map(),
        m_background_renderer->scene(),
        m_rendering_config.graph,
        ARCHV_RESOURCE_GROUP);

    m_graph_renderer->render_scaling(make_scale_map());
    m_graph_renderer->render_weights(make_weight_map());
    m_graph_renderer->render_in_degree_effects();
    m_graph_renderer->render_out_degree_effects();

    BOOST_LOG_TRIVIAL(info) << "setup graph rendering";
}

auto app::setup_gui() -> void
{
    setup_gui_overlay();
    setup_gui_tray_manager();
    install_gui_plugins();
    create_gui();
    setup_gui_undo_redo();
    setup_gui_background_configurator();
    setup_gui_graph_configurator();
    setup_gui_gui_configurator();
    setup_gui_dependencies_editor();
    setup_gui_layout_editor();
    setup_gui_scaling_editor();

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

auto app::setup_gui_overlay() -> void
{
    auto* imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui); // takes ownership

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    m_background_renderer->scene().addRenderQueueListener(ogre_overlay);
}

auto app::setup_gui_tray_manager() -> void
{
    m_tray_manager = std::make_unique< OgreBites::TrayManager >(
        "TrayManager", getRenderWindow());

    m_tray_manager->showCursor();
    addInputListener(m_tray_manager.get());
}

namespace
{
    inline auto from_archv_group(const Ogre::ResourcePtr& ptr)
    {
        return ptr->getGroup() == ARCHV_RESOURCE_GROUP;
    }

    auto load_gui_resources(Ogre::ResourceManager& manager)
    {
        using resources_vector = std::vector< std::string_view >;

        auto range = manager.getResourceIterator();

        // clang-format off
        auto ptrs_range = std::ranges::views::values(range)
                        | std::ranges::views::filter(from_archv_group);
        // clang-format on

        auto resources = resources_vector();

        std::transform(
            std::begin(ptrs_range),
            std::end(ptrs_range),
            std::back_inserter(resources),
            [](const auto& ptr) { return ptr->getName().c_str(); });

        std::sort(std::begin(resources), std::end(resources)); // alphabetically

        return resources;
    }

    inline auto load_gui_meshes()
    {
        auto res = gui::resources::meshes_vector();

        const auto& mnger = Ogre::ResourceGroupManager::getSingleton();
        static const auto meshes_ptr
            = mnger.findResourceNames(ARCHV_RESOURCE_GROUP, "*.mesh");

        assert(meshes_ptr);

        std::transform(
            std::cbegin(*meshes_ptr),
            std::cend(*meshes_ptr),
            std::back_inserter(res),
            [](const auto& mesh) { return mesh.c_str(); });

        std::sort(std::begin(res), std::end(res)); // alphabetically

        gui::resources::load_meshes(std::move(res));

        BOOST_LOG_TRIVIAL(debug) << "loaded gui meshes";
    }

    inline auto load_gui_materials()
    {
        using namespace Ogre;
        auto&& materials = load_gui_resources(MaterialManager::getSingleton());
        gui::resources::load_materials(std::move(materials));

        BOOST_LOG_TRIVIAL(debug) << "loaded gui materials";
    }

    inline auto load_gui_fonts()
    {
        using namespace Ogre;
        auto&& fonts = load_gui_resources(FontManager::getSingleton());
        gui::resources::load_fonts(std::move(fonts));

        BOOST_LOG_TRIVIAL(debug) << "loaded gui fonts";
    }

} // namespace

auto app::load_gui_resources() -> void
{
    load_gui_meshes();
    load_gui_materials();
    load_gui_fonts();

    BOOST_LOG_TRIVIAL(debug) << "loaded gui resources";
}

auto app::install_gui_plugins() -> void
{
    gui::plugins::install_dependencies(
        [this]()
        {
            auto res = gui::plugins::dependency_set();
            const auto& plugged
                = std::ranges::views::keys(m_dependencies_config);

            std::transform(
                std::cbegin(plugged),
                std::cend(plugged),
                std::inserter(res, std::begin(res)),
                [](const auto& dep) { return dep.c_str(); });

            return res;
        }());

    gui::plugins::install_layouts(
        [this]()
        {
            auto res = gui::plugins::layout_set();
            const auto& plugged = m_layout_config.layouts;

            std::transform(
                std::cbegin(plugged),
                std::cend(plugged),
                std::inserter(res, std::begin(res)),
                [](const auto& lay) { return lay.c_str(); });

            return res;
        }());

    gui::plugins::install_topologies(
        { [this]()
          {
              auto res = gui::plugins::topology_set();
              const auto& plugged = m_layout_config.topologies;

              std::transform(
                  std::cbegin(plugged),
                  std::cend(plugged),
                  std::inserter(res, std::begin(res)),
                  [](const auto& space) { return space.c_str(); });

              return res;
          }() });

    gui::plugins::install_scales(m_layout_config.scales);

    gui::plugins::install_factors(
        [this]()
        {
            auto res = gui::plugins::factor_set();
            const auto& plugged = std::ranges::views::keys(m_scaling_config);

            std::transform(
                std::cbegin(plugged),
                std::cend(plugged),
                std::inserter(res, std::begin(res)),
                [](const auto& factor) { return factor.c_str(); });

            return res;
        }());

    BOOST_LOG_TRIVIAL(debug) << "installed gui pluggins";
}

auto app::create_gui() -> void
{
    m_gui = std::make_unique< gui::gui >(m_gui_config);

    BOOST_LOG_TRIVIAL(debug) << "created gui";
}

auto app::setup_gui_undo_redo() -> void
{
    assert(m_gui);

    m_gui->get_editor().set_undo_enabled([this]()
                                         { return m_commands->can_undo(); });

    m_gui->get_editor().set_redo_enabled([this]()
                                         { return m_commands->can_redo(); });

    BOOST_LOG_TRIVIAL(debug) << "setup gui hooks";
}

namespace
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        return std::array { val.r, val.g, val.b, val.a };
    }

    inline auto to_scale(const Ogre::Vector3& vec)
    {
        return std::array { vec.x, vec.y, vec.z };
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

    BOOST_LOG_TRIVIAL(debug) << "setup gui background values";
}

auto app::setup_gui_graph_configurator() -> void
{
    assert(m_gui);

    auto& graph_gui = m_gui->get_configurator().get_graph_configurator();
    const auto& cfg = m_rendering_config.graph;

    graph_gui.set_node_mesh(cfg.vertex_mesh);
    graph_gui.set_node_scale(to_scale(cfg.vertex_scale));
    graph_gui.set_node_font(cfg.vertex_id_font_name);
    graph_gui.set_node_font_color(to_rgba(cfg.vertex_id_color));
    graph_gui.set_node_char_height(cfg.vertex_id_char_height);
    graph_gui.set_node_space_width(cfg.vertex_id_space_width);
    graph_gui.set_edge_material(cfg.edge_material);
    graph_gui.set_edge_tip_mesh(cfg.edge_tip_mesh);
    graph_gui.set_edge_tip_scale(to_scale(cfg.edge_tip_scale));
    graph_gui.set_edge_font(cfg.edge_type_font_name);
    graph_gui.set_edge_font_color(to_rgba(cfg.edge_type_color));
    graph_gui.set_edge_char_height(cfg.edge_type_char_height);
    graph_gui.set_edge_space_width(cfg.edge_type_space_width);

    BOOST_LOG_TRIVIAL(debug) << "setup gui graph values";
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

    BOOST_LOG_TRIVIAL(debug) << "setup gui gui values";
}

auto app::setup_gui_layout_editor() -> void
{
    assert(m_gui);
    assert(m_layout_backend);

    m_gui->get_editor().get_layout_editor().set_layout(
        [this]() { return layout::identify(m_layout_backend->get_layout()); });

    m_gui->get_editor().get_layout_editor().set_topology(
        [this]()
        { return layout::identify(m_layout_backend->get_topology()); });

    m_gui->get_editor().get_layout_editor().set_scale(
        [this]() { return m_layout_backend->get_topology().scale(); });

    BOOST_LOG_TRIVIAL(debug) << "setup gui layout editor";
}

auto app::setup_gui_dependencies_editor() -> void
{
    assert(m_gui);
    assert(m_dependencies_backend);

    m_gui->get_editor().get_dependencies_editor().set_weights(
        [this](auto dep)
        { return m_dependencies_backend->get_weight_repo().get_weight(dep); });

    BOOST_LOG_TRIVIAL(debug) << "setup gui dependencies editor";
}

auto app::setup_gui_scaling_editor() -> void
{
    assert(m_gui);
    assert(m_scaling_backend);

    m_gui->get_editor().get_scaling_editor().set_dims(
        [this](auto tag)
        { return scaling::get_factor_dims(*m_scaling_backend, tag); });

    m_gui->get_editor().get_scaling_editor().set_baseline(
        [this](auto tag)
        { return scaling::get_factor_baseline(*m_scaling_backend, tag); });

    m_gui->get_editor().get_scaling_editor().set_enabled(
        [this](auto tag)
        { return scaling::is_factor_enabled(*m_scaling_backend, tag); });

    m_gui->get_editor().get_scaling_editor().set_min_ratio(
        [this](auto tag)
        { return scaling::get_factor_min_ratio(*m_scaling_backend, tag); });

    m_gui->get_editor().get_scaling_editor().set_max_ratio(
        [this](auto tag)
        { return scaling::get_factor_max_ratio(*m_scaling_backend, tag); });

    BOOST_LOG_TRIVIAL(debug) << "setup gui scaling editor";
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
    m_background_renderer->scene().removeRenderQueueListener(ogre_overlay);

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