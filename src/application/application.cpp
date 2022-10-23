#include "application.hpp"

#include "archive.hpp"
#include "commands.hpp"
#include "config.hpp"
#include "ui/all.hpp"

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
    
// TODO Show ogre config dialog

app::app(int argc, const char* argv[]) : base("ARCHV")
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

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

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

    BOOST_LOG_TRIVIAL(debug) << "setup";
}

auto app::shutdown() -> void
{
    shutdown_input();
    shutdown_gui();
    shutdown_graph_renderer();
    shutdown_background_renderer();
    shutdown_commands();
    shutdown_graph_interface();

    BOOST_LOG_TRIVIAL(debug) << "shutdown";
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

    BOOST_LOG_TRIVIAL(debug) << "setup graph interface";
}

auto app::setup_commands() -> void
{
    m_cmds = std::make_unique< command_history_type >();

    BOOST_LOG_TRIVIAL(debug) << "setup commands";
}

auto app::setup_background_renderer() -> void
{
    m_background_renderer = std::make_unique< background_renderer_type >(
        *getRenderWindow(),
        m_rendering_config.background,
        ARCHV_RESOURCE_GROUP);

    BOOST_LOG_TRIVIAL(debug) << "setup background renderer";
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

    BOOST_LOG_TRIVIAL(debug) << "setup graph renderer";
}

namespace // gui setup
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
    }

    inline auto load_gui_materials()
    {
        using namespace Ogre;
        auto&& materials = load_gui_resources(MaterialManager::getSingleton());
        gui::resources::load_materials(std::move(materials));
    }

    inline auto load_gui_fonts()
    {
        using namespace Ogre;
        auto&& fonts = load_gui_resources(FontManager::getSingleton());
        gui::resources::load_fonts(std::move(fonts));
    }

    inline auto load_gui_resources()
    {
        load_gui_meshes();
        load_gui_materials();
        load_gui_fonts();
    }

    inline auto setup_gui_overlay()
    {
        auto* imgui = new Ogre::ImGuiOverlay();
        imgui->show();
        assert(imgui->isInitialised());
        // takes ownership
        Ogre::OverlayManager::getSingleton().addOverlay(imgui);
    }

} // namespace

auto app::setup_gui() -> void
{
    install_gui_plugins(m_weights_config, m_layout_config, m_scaling_config);
    load_gui_resources();

    setup_gui_overlay();
    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    assert(ogre_overlay);
    m_background_renderer->scene().addRenderQueueListener(ogre_overlay);

    m_gui = std::make_unique< gui_type >(m_gui_config);

    BOOST_LOG_TRIVIAL(debug) << "setup gui";
}

auto app::setup_input() -> void
{
    m_tray = std::make_unique< tray_type >("Bob", getRenderWindow());
    m_tray->showCursor();

    m_cameraman = std::make_unique< cameraman_type >(
        &m_background_renderer->cam_node());

    m_gui_input_handler = std::make_unique< gui_input_handler_type >();

    m_hud_input_handler = std::make_unique< hud_input_handler_type >(*m_tray);

    m_pause_resume_handler = std::make_unique< pause_resume_handler_type >(
        *this, *m_cameraman, *m_gui_input_handler);

    m_quit_handler = std::make_unique< quit_handler_type >(*getRoot());

    m_shortcut_input_handler
        = std::make_unique< shortcut_input_handler_type >(*m_gui);

    addInputListener(m_tray.get());
    addInputListener(m_cameraman.get());
    addInputListener(m_hud_input_handler.get());
    addInputListener(m_pause_resume_handler.get());
    addInputListener(m_quit_handler.get());
    addInputListener(m_shortcut_input_handler.get());

#ifdef NDEBUG
    SDL_SetRelativeMouseMode(SDL_TRUE);
#else
    SDL_ShowCursor(SDL_FALSE);
#endif

    BOOST_LOG_TRIVIAL(debug) << "setup input";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown_input() -> void
{
    removeInputListener(m_shortcut_input_handler.get());
    removeInputListener(m_quit_handler.get());
    removeInputListener(m_pause_resume_handler.get());
    removeInputListener(m_hud_input_handler.get());
    removeInputListener(m_cameraman.get());
    removeInputListener(m_tray.get());

    m_shortcut_input_handler.reset();
    m_quit_handler.reset();
    m_pause_resume_handler.reset();
    m_hud_input_handler.reset();
    m_cameraman.reset();
    m_tray.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown input";
}

auto app::shutdown_gui() -> void
{
    m_gui.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown gui";
}

auto app::shutdown_graph_renderer() -> void
{
    m_graph_renderer.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown graph renderer";
}

auto app::shutdown_background_renderer() -> void
{
    m_background_renderer.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown background renderer";
}

auto app::shutdown_commands() -> void
{
    m_cmds.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown commands";
}

auto app::shutdown_graph_interface() -> void
{
    m_graph_iface.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown graph interface";
}

/***********************************************************
 * GUI Prepare                                             *
 ***********************************************************/

auto app::prepare_weights_editor() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    m_gui->get_weights_editor().set_weights(
        [this](auto type) {
            return weights::get_weight(m_graph_iface->weights_backend(), type);
        });

    BOOST_LOG_TRIVIAL(debug) << "prepared weights editor";
}

auto app::prepare_layout_editor() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    m_gui->get_layout_editor().set_layout(
        [this]()
        { return layout::get_layout_id(m_graph_iface->layout_backend()); });

    m_gui->get_layout_editor().set_topology(
        [this]()
        { return layout::get_topology_id(m_graph_iface->layout_backend()); });

    m_gui->get_layout_editor().set_scale(
        [this]()
        { return layout::get_scale(m_graph_iface->layout_backend()); });

    BOOST_LOG_TRIVIAL(debug) << "prepared layout editor";
}

auto app::prepare_scaling_editor() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    m_gui->get_scaling_editor().set_dims(
        [this](auto tag) {
            return scaling::get_factor_dims(
                m_graph_iface->scaling_backend(), tag);
        });

    m_gui->get_scaling_editor().set_baseline(
        [this](auto tag)
        {
            return scaling::get_factor_baseline(
                m_graph_iface->scaling_backend(), tag);
        });

    m_gui->get_scaling_editor().set_enabled(
        [this](auto tag) {
            return scaling::is_factor_enabled(
                m_graph_iface->scaling_backend(), tag);
        });

    m_gui->get_scaling_editor().set_min_ratio(
        [this](auto tag)
        {
            return scaling::get_factor_min_ratio(
                m_graph_iface->scaling_backend(), tag);
        });

    m_gui->get_scaling_editor().set_max_ratio(
        [this](auto tag)
        {
            return scaling::get_factor_max_ratio(
                m_graph_iface->scaling_backend(), tag);
        });

    BOOST_LOG_TRIVIAL(debug) << "prepared scaling editor";
}

namespace // presentation translations
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        return std::array { val.r, val.g, val.b, val.a };
    }

    inline auto to_color(const std::array< float, 4 >& rgba)
    {
        return Ogre::ColourValue(rgba[0], rgba[1], rgba[2], rgba[3]);
    }

    inline auto to_scale(const Ogre::Vector3& vec)
    {
        return std::array { vec.x, vec.y, vec.z };
    }

    inline auto to_vec3(const std::array< float, 3 >& scale)
    {
        return Ogre::Vector3(scale[0], scale[1], scale[2]);
    }

} // namespace

auto app::prepare_background_configurator() -> void
{
    assert(m_gui);
    assert(m_background_renderer);

    auto& bkg_gui = m_gui->get_bkg_configurator();
    const auto& cfg = m_background_renderer->config_data();

    bkg_gui.set_skybox_material(cfg.skybox_material);
    bkg_gui.set_skybox_distance(cfg.skybox_distance);
    bkg_gui.set_ambient_color(to_rgba(cfg.ambient_color));
    bkg_gui.set_diffuse_color(to_rgba(cfg.diffuse_color));
    bkg_gui.set_specular_color(to_rgba(cfg.specular_color));
    bkg_gui.set_cam_far_clip_distance(cfg.cam_far_clip_distance);
    bkg_gui.set_cam_near_clip_distance(cfg.cam_near_clip_distance);

    BOOST_LOG_TRIVIAL(debug) << "prepared background configurator";
}

auto app::prepare_graph_configurator() -> void
{
    assert(m_gui);
    assert(m_graph_renderer);

    auto& graph_gui = m_gui->get_graph_configurator();
    const auto& cfg = m_graph_renderer->config_data();

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

    BOOST_LOG_TRIVIAL(debug) << "prepared graph configurator";
}

auto app::prepare_gui_configurator() -> void
{
    assert(m_gui);

    auto& gui_configurator = m_gui->get_gui_configurator();
    const auto& cfg = m_gui->config_data();

    gui_configurator.set_color_theme(cfg.color_theme);
    gui_configurator.set_frame_rounding(cfg.frame_rounding);
    gui_configurator.set_frame_bordered(cfg.frame_bordered);
    gui_configurator.set_window_bordered(cfg.window_bordered);
    gui_configurator.set_popup_bordered(cfg.popup_bordered);

    BOOST_LOG_TRIVIAL(debug) << "prepared gui configurator";
}

auto app::prepare_gui_undo_redo() -> void
{
    assert(m_gui);

    m_gui->set_can_undo([this]() { return m_cmds->can_undo(); });
    m_gui->set_can_redo([this]() { return m_cmds->can_redo(); });

    BOOST_LOG_TRIVIAL(debug) << "prepared undo redo gui";
}

/***********************************************************
 * Presentation Connections                                *
 ***********************************************************/

auto app::connect_weights_presentation() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    auto& editor = m_gui->get_weights_editor();
    auto& backend = m_graph_iface->weights_backend();

    editor.connect_to_dependency(
        [this, &backend](auto type, auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected: " << w << " for: " << type;
            commands::update_dependency_weight(*m_cmds, backend, type, w);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected weights restore";
            commands::restore_weights(*m_cmds, backend);
        });

    backend.connect(
        [this](auto, auto)
        {
            m_graph_renderer->render_weights(m_graph_iface->edge_weight());
            BOOST_LOG_TRIVIAL(info) << "rendered edge weights";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected weights presentation";
}

auto app::connect_layout_presentation() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    auto& editor = m_gui->get_layout_editor();
    auto& backend = m_graph_iface->layout_backend();

    editor.connect_to_layout(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout: " << selection;
            commands::update_layout(*m_cmds, backend, selection);
        });

    editor.connect_to_topology(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected topology: " << selection;
            commands::update_layout_topology(*m_cmds, backend, selection);
        });

    editor.connect_to_scale(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scale: " << selection;
            commands::update_layout_scale(*m_cmds, backend, selection);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout restore";
            commands::restore_layout(*m_cmds, backend);
        });

    backend.connect_to_layout(
        [this](const auto&)
        {
            m_graph_renderer->render_layout(m_graph_iface->vertex_position());
            BOOST_LOG_TRIVIAL(info) << "rendered layout";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected layout presentation";
}

auto app::connect_scaling_presentation() -> void
{
    assert(m_gui);
    assert(m_graph_iface);

    auto& editor = m_gui->get_scaling_editor();
    auto& backend = m_graph_iface->scaling_backend();

    editor.connect_to_baseline(
        [this, &backend](auto tag, auto b)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected baseline: " << baseline << "for: " << tag;
            commands::update_scaling_factor_baseline(
                *m_cmds, backend, tag, baseline);
        });

    editor.connect_to_dims(
        [this, &backend](auto tag, const auto& dims)
        {
            BOOST_LOG_TRIVIAL(info) << "selected new dims for: " << tag;
            commands::update_scaling_factor_dims(*m_cmds, backend, tag, dims);
        });

    editor.connect_to_enabled(
        [this, &backend](auto tag, auto enabled)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected enabled: " << enabled << " for: " << tag;
            commands::update_scaling_factor_enablement(
                *m_cmds, backend, tag, enabled);
        });

    editor.connect_to_min_ratio(
        [this, &backend](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected min ratio: " << ratio << " for: " << tag;
            commands::update_scaling_factor_min_ratio(
                *m_cmds, backend, tag, ratio);
        });

    editor.connect_to_max_ratio(
        [this, &backend](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected max ratio: " << ratio << " for: " << tag;
            commands::update_scaling_factor_max_ratio(
                *m_cmds, backend, tag, ratio);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling restore";
            commands::restore_scaling(*m_cmds, backend);
        });

    backend.connect(
        [this](const auto&)
        {
            m_graph_renderer->render_scaling(m_graph_iface->vertex_scale());
            BOOST_LOG_TRIVIAL(info) << "rendered vertex scaling";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected scaling presentation";
}

auto app::connect_background_presentation() -> void
{
    assert(m_gui);
    assert(m_background_renderer);

    auto& iface = m_gui->get_bkg_configurator();
    auto& api = m_background_renderer->config_api();

    iface.connect_to_skybox_material(
        [this, &api](auto material)
        {
            BOOST_LOG_TRIVIAL(info) << "selected skybox material: " << material;
            api.set_skybox_material(std::string(material));
        });

    iface.connect_to_skybox_distance(
        [this, &api](auto dist)
        {
            BOOST_LOG_TRIVIAL(info) << "selected skybox distance: " << dist;
            api.set_skybox_distance(dist);
        });

    iface.connect_to_ambient_color(
        [this, &api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected ambient color";
            api.set_ambient_color(to_color(col));
        });

    iface.connect_to_diffuse_color(
        [this, &api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected diffuse color";
            api.set_diffuse_color(to_color(col));
        });

    iface.connect_to_specular_color(
        [this, &api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected specular color";
            api.set_specular_color(to_color(col));
        });

    iface.connect_to_cam_far_clip_distance(
        [this, &api](auto dist)
        {
            BOOST_LOG_TRIVIAL(info) << "selected far clip distance: " << dist;
            api.set_cam_far_clip_distance(dist);
        });

    iface.connect_to_cam_near_clip_distance(
        [this, &api](auto dist)
        {
            BOOST_LOG_TRIVIAL(info) << "selected near clip distance: " << dist;
            api.set_cam_near_clip_distance(dist);
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background apply";
            ui::apply_configs(*m_background_renderer);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background preview";
            ui::begin_preview(*m_background_renderer);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background cancel";
            ui::end_preview(*m_background_renderer);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background restore";
            ui::restore_defaults(*m_background_renderer);
            prepare_background_configurator();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected background presentation";
}

auto app::connect_graph_presentation() -> void
{
    assert(m_gui);
    assert(m_graph_renderer);

    auto& iface = m_gui->get_graph_configurator();
    auto& api = m_graph_renderer->config_api();

    iface.connect_to_node_mesh(
        [this, &api](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node mesh: " << mesh;
            api.set_vertex_mesh(std::string(mesh));
        });

    iface.connect_to_node_scale(
        [this, &api](const auto& scale)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node new scale";
            api.set_vertex_scale(to_vec3(scale));
        });

    iface.connect_to_node_font(
        [this, &api](auto font)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node font: " << font;
            api.set_vertex_id_font_name(std::string(font));
        });

    iface.connect_to_node_font_color(
        [this, &api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node new font color";
            api.set_vertex_id_color(to_color(col));
        });

    iface.connect_to_node_char_height(
        [this, &api](auto h)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node char height: " << h;
            api.set_vertex_id_char_height(h);
        });

    iface.connect_to_node_space_width(
        [this, &api](auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node space width: " << w;
            api.set_vertex_space_width(w);
        });

    iface.connect_to_edge_material(
        [this, &api](auto mat)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge material: " << mat;
            api.set_edge_material(std::string(material));
        });

    iface.connect_to_edge_tip_mesh(
        [this, &api](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge tip mesh: " << mesh;
            api.set_edge_tip_mesh(std::string(mesh));
        });

    iface.connect_to_edge_tip_scale(
        [this, &api](const auto& scale)
        {
            BOOST_LOG_TRIVIAL(info) << "selected new edge tip scale";
            api.set_edge_tip_scale(to_vec3(scale));
        });

    iface.connect_to_edge_font(
        [this, &api](auto font)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge type font: " << font;
            api.set_edge_type_font_name(std::string(name));
        });

    iface.connect_to_edge_font_color(
        [this, &api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected new edge type font color";
            api.set_edge_type_color(to_color(col));
        });

    iface.connect_to_edge_char_height(
        [this, &api](auto h)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge type char height: " << h;
            api.set_edge_type_char_height(h);
        });

    iface.connect_to_edge_space_width(
        [this, &api](auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge type space width: " << w;
            api.set_edge_type_space_width(w);
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph apply";
            ui::apply_configs(*m_graph_renderer);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph preview";
            ui::begin_preview(*m_graph_renderer);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph cancel";
            ui::end_preview(*m_graph_renderer);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph restore";
            ui::restore_defaults(*m_graph_renderer);
            prepare_graph_configurator();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected graph presentation";
}

auto app::connect_gui_presentation() -> void
{
    assert(m_gui);

    auto& iface = m_gui->get_gui_configurator();
    auto& api = m_gui->config_api();

    iface.connect_to_color_theme(
        [this, &api](auto theme)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui theme: " << theme;
            api.set_color_theme(std::string(theme));
        });

    iface.connect_to_frame_rounding(
        [this, &api](auto r)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame rounding: " << r;
            api.set_frame_rounding(r);
        });

    iface.connect_to_window_bordered(
        [this, &api](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui window bordered: " << b;
            api.set_window_bordered(b);
        });

    iface.connect_to_frame_bordered(
        [this, &api](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame bordered: " << b;
            api.set_frame_bordered(b);
        });

    iface.connect_to_popup_bordered(
        [this, &api](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui popup bordered: " << b;
            api.set_popup_bordered(b);
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui apply";
            ui::apply_configs(*m_gui);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui preview";
            ui::begin_preview(*m_gui);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui cancel";
            ui::end_preview(*m_gui);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui restore";
            ui::restore_defaults(*m_gui);
            prepare_gui_configurator();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected gui presentation";
}

auto app::connect_undo_redo_presentation() -> void
{
    assert(m_gui);

    m_gui->connect_to_undo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected undo";
            m_cmds->undo();
        });

    m_gui->connect_to_redo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected redo";
            m_cmds->redo();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected undo redo presentation";
}

} // namespace application