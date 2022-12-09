#include "application.hpp"

#include "IconsFontAwesome5.h"
#include "archive.hpp"
#include "config.hpp"
#include "input/all.hpp"
#include "misc/all.hpp"
#include "presentation/all.hpp"
#include "ui/all.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Bites/OgreTrays.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreParticleSystemManager.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>
#include <map>
#include <ranges>

namespace application
{

namespace pres = presentation;

application::application(int argc, const char* argv[]) : base("ARCHV")
{
    if (argc != 2)
    {
        std::cout << "usage: `./<exec> <path/to/graph.json>`\n";
        std::exit(EXIT_FAILURE);
    }

    m_graph_path = argv[1];
}

application::~application() = default;

auto application::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    base::frameStarted(e);
    Ogre::ImGuiOverlay::NewFrame();
    //  ImGui::ShowDemoWindow();
    gui::render(*m_gui);

    return true;
}

auto application::go() -> void
{
    getRoot()->startRendering();
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto application::setup() -> void
{
    base::setup();
    setup_graph_interface();
    setup_commands();
    setup_rendering();
    setup_gui();
    setup_input();

    prepare_weights_editor();
    prepare_layout_editor();
    prepare_scaling_editor();
    prepare_degrees_editor();
    prepare_clustering_editor();
    prepare_background_configurator();
    prepare_graph_configurator();
    prepare_minimap_configurator();
    prepare_gui_configurator();
    prepare_menu_bar();

    connect_weights_presentation();
    connect_layout_presentation();
    connect_scaling_presentation();
    connect_degrees_presentation();
    connect_clustering_presentation();
    connect_background_presentation();
    connect_graph_presentation();
    connect_minimap_presentation();
    connect_gui_presentation();
    connect_menu_bar_presentation();

    BOOST_LOG_TRIVIAL(debug) << "setup";
}

auto application::setup_graph_interface() -> void
{
    const auto& jsons = archive::get();

    auto&& [st, g, m] = architecture::deserialize(jsons.at(m_graph_path));

    const auto& weights_root = jsons.at(ARCHV_WEIGHTS_CONFIG_PATH);
    const auto& layout_root = jsons.at(ARCHV_LAYOUT_CONFIG_PATH);
    const auto& scaling_root = jsons.at(ARCHV_SCALING_CONFIG_PATH);
    const auto& clustering_root = jsons.at(ARCHV_CLUSTERING_CONFIG_PATH);

    m_graph_iface = std::make_unique< graph_interface_type >(
        std::move(st),
        std::move(g),
        std::move(m),
        weights::deserialize(weights_root),
        layout::deserialize(layout_root),
        scaling::deserialize(scaling_root),
        clustering::deserialize(clustering_root));

    BOOST_LOG_TRIVIAL(debug) << "setup graph interface";
}

auto application::setup_commands() -> void
{
    m_cmds = std::make_unique< command_history_type >();

    BOOST_LOG_TRIVIAL(debug) << "setup commands";
}

auto application::setup_rendering() -> void
{
    using degrees_evaluator = rendering::degrees_ranked_evaluator;
    using degrees_backend = rendering::degrees_ranked_backend;
    using cluster_color_coder = rendering::cluster_color_pool;

    const auto& jsons = archive::get();
    const auto& root = jsons.at(ARCHV_RENDERING_CONFIG_PATH);
    const auto config = rendering::deserialize(root);

    m_background_renderer = std::make_unique< background_renderer_type >(
        *getRenderWindow(), config.background, ARCHV_RESOURCE_GROUP);

    m_graph_renderer = std::make_unique< graph_renderer_type >(
        m_graph_iface->get_graph(),
        pres::vertex_id(*m_graph_iface),
        pres::vertex_position(*m_graph_iface),
        pres::edge_dependency(*m_graph_iface),
        m_background_renderer->scene(),
        config.graph,
        ARCHV_RESOURCE_GROUP,
        degrees_evaluator(degrees_backend(config.degrees)),
        cluster_color_coder());

    m_graph_renderer->render_scaling(pres::vertex_scale(*m_graph_iface));
    m_graph_renderer->render_weights(pres::edge_weight(*m_graph_iface));
    m_graph_renderer->render_in_degree_particles();
    m_graph_renderer->render_out_degree_particles();

    m_graph_collisions = std::make_unique< graph_collision_checker_type >(
        m_graph_iface->get_graph(),
        pres::vertex_id(*m_graph_iface),
        pres::edge_dependency(*m_graph_iface),
        m_background_renderer->scene());

    m_minimap_renderer = std::make_unique< minimap_renderer_type >(
        *getRenderWindow(),
        m_background_renderer->scene(),
        config.minimap,
        ARCHV_RESOURCE_GROUP);

    m_minimap_renderer->setup();

    BOOST_LOG_TRIVIAL(debug) << "setup rendering";
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

    inline auto prepare_gui_plugins(
        const weights::config_data& wcfg,
        const layout::config_data& lcf,
        const scaling::config_data& scfg,
        const clustering::config_data& ccfg)
    {
        using std::ranges::views::keys;
        gui::plugins::install_dependencies(to_plugin_set(keys(wcfg)));
        gui::plugins::install_layouts(to_plugin_set(lcf.layouts));
        gui::plugins::install_topologies(to_plugin_set(lcf.topologies));
        gui::plugins::install_factors(to_plugin_set(keys(scfg)));
        gui::plugins::install_clusterers(to_plugin_set(ccfg.clusterers));
        gui::plugins::install_mst_finders(to_plugin_set(ccfg.mst_finders));
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

    inline auto load_gui_particles()
    {
        using namespace Ogre;

        auto& particles = ParticleSystemManager::getSingleton();

        static auto templates = particles.getTemplateIterator();

        auto systems = gui::resources::particle_systems_vector();

        std::transform(
            std::begin(templates),
            std::end(templates),
            std::back_inserter(systems),
            [](const auto& pair) { return pair.first.c_str(); });

        gui::resources::load_particle_systems(std::move(systems));
    }

    inline auto prepare_gui_resources()
    {
        load_gui_meshes();
        load_gui_materials();
        load_gui_fonts();
        load_gui_particles();
    }

    // TODO(?) Load from config?
    // NOTE: Must be called befor overlay->show
    auto prepare_font_icons() -> void
    {
        using misc::resolve_source_path;

        assert(ImGui::GetCurrentContext());

        auto& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();

        static const ImWchar ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;

        const auto fonts = resolve_source_path("../../data/fonts/").string();
        const auto fas = fonts + FONT_ICON_FILE_NAME_FAS;
        const auto far = fonts + FONT_ICON_FILE_NAME_FAR;

        io.Fonts->AddFontFromFileTTF(fas.c_str(), 14.0f, &config, ranges);
        io.Fonts->AddFontFromFileTTF(far.c_str(), 14.0f, &config, ranges);
    }

    // Named by Ogre Overlay component.
    constexpr auto imgui_overlay_name = "ImGuiOverlay";

    inline auto prepare_gui_overlay(Ogre::SceneManager& scene)
    {
        auto* overlay = new Ogre::ImGuiOverlay();
        prepare_font_icons();
        overlay->show();
        assert(overlay->isInitialised());
        // takes ownership
        Ogre::OverlayManager::getSingleton().addOverlay(overlay);

        auto* system = Ogre::OverlaySystem::getSingletonPtr();
        assert(system);
        scene.addRenderQueueListener(system);
    }

    inline auto string_views(const std::vector< std::string >& strs)
    {
        using result_type = std::vector< std::string_view >;
        auto res = result_type { std::cbegin(strs), std::cend(strs) };
        std::sort(std::begin(res), std::end(res));
        return res;
    }

    template < architecture::symbol_composed T >
    inline auto string_views(const std::vector< T >& syms)
    {
        using result_type = std::vector< std::string_view >;
        result_type res;
        std::transform(
            std::cbegin(syms),
            std::cend(syms),
            std::back_inserter(res),
            [](const auto& s) { return s.sym.id.c_str(); });
        std::sort(std::begin(res), std::end(res));
        return res;
    }

    inline auto structure_info(const architecture::structure& s)
    {
        return gui::structure_info { .id = s.sym.id,
                                     .name = s.sym.name,
                                     .t = s.t,
                                     .name_space = s.sym.name_space,
                                     .file = s.sym.source.file,
                                     .methods = string_views(s.methods),
                                     .fields = string_views(s.fields),
                                     .bases = string_views(s.bases),
                                     .nested = string_views(s.nested),
                                     .friends = string_views(s.friends) };
    }

    auto structure_popup(const architecture::structure& s)
    {
        return gui::structure_popup(structure_info(s));
    }

    auto prepare_gui_popups(
        const architecture::symbol_table& st,
        gui::popup_holder& popups,
        gui::overlay_manager& overlays)
    {
        for (const auto& [id, sym] : st)
        {
            auto&& popup = structure_popup(sym);
            popups.insert(id, std::move(popup));
            overlays.submit(&popups.get(id)); // Queue for rendering
        }
    }

} // namespace

auto application::setup_gui() -> void
{
    prepare_gui_plugins(
        m_graph_iface->get_weights_backend().config_data(),
        m_graph_iface->get_layout_backend().config_data(),
        m_graph_iface->get_scaling_backend().config_data(),
        m_graph_iface->get_clustering_backend().config_data());

    prepare_gui_resources();
    prepare_gui_overlay(m_background_renderer->scene());

    ImGui::GetIO().WantCaptureMouse = true;

    const auto& gui_root = archive::get().at(ARCHV_GUI_CONFIG_PATH);
    gui::set_configs(gui::deserialize(gui_root));

    m_gui = std::make_unique< gui_type >();

    prepare_gui_popups(m_graph_iface->get_symbol_table(), gui::popups, *m_gui);

    BOOST_LOG_TRIVIAL(debug) << "setup gui";
}

auto application::setup_input() -> void
{
    using std::make_unique;
    using listeners_vec = std::vector< OgreBites::InputListener* >;

    m_trays = std::make_unique< trays_type >("Bob", getRenderWindow());
    m_trays->showCursor();

    m_cameraman = make_unique< cameraman_type >(
        OgreBites::CameraMan(&m_background_renderer->cam_node()));

    m_gui_input_handler = make_unique< gui_input_handler_type >();

    m_hud_input_handler = make_unique< hud_input_handler_type >(
        m_gui->get_controls_hud(),
        m_gui->get_frames_hud(),
        *m_minimap_renderer);

    m_quit_handler = make_unique< quit_handler_type >(*getRoot());

    m_shortcut_input_handler
        = make_unique< shortcut_input_handler_type >(*m_cmds);

    m_inspection_input_handler = make_unique< inspection_handler_type >(
        *m_graph_renderer,
        *m_graph_collisions,
        *m_gui,
        gui::popups,
        *getRenderWindow(),
        m_background_renderer->cam());

    auto&& listeners = listeners_vec { { m_gui_input_handler.get(),
                                         m_hud_input_handler.get(),
                                         m_cameraman.get(),
                                         m_inspection_input_handler.get() } };

    m_input_chain = make_unique< event_dispatcher_type >(std::move(listeners));

    addInputListener(m_trays.get());
    addInputListener(m_quit_handler.get());
    addInputListener(m_shortcut_input_handler.get());
    addInputListener(m_input_chain.get());

#ifdef NDEBUG
    SDL_SetRelativeMouseMode(SDL_TRUE); // Constrain mouse in application
#else
    SDL_ShowCursor(SDL_FALSE);
#endif

    BOOST_LOG_TRIVIAL(debug) << "setup input";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto application::shutdown() -> void
{
    shutdown_input();
    shutdown_gui();
    shutdown_rendering();
    shutdown_commands();
    shutdown_graph_interface();
    base::shutdown();

    BOOST_LOG_TRIVIAL(debug) << "shutdown";
}

auto application::shutdown_input() -> void
{
    removeInputListener(m_input_chain.get());
    removeInputListener(m_trays.get());
    removeInputListener(m_shortcut_input_handler.get());
    removeInputListener(m_quit_handler.get());

    m_input_chain.reset();
    m_trays.reset();
    m_inspection_input_handler.reset();
    m_shortcut_input_handler.reset();
    m_quit_handler.reset();
    m_gui_input_handler.reset();
    m_hud_input_handler.reset();
    m_cameraman.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown input";
}

auto application::shutdown_gui() -> void
{
    m_gui.reset();
    gui::popups.clear();
    Ogre::OverlayManager::getSingleton().destroy(imgui_overlay_name);

    BOOST_LOG_TRIVIAL(debug) << "shutdown gui";
}

auto application::shutdown_rendering() -> void
{
    m_minimap_renderer.reset();
    m_graph_collisions.reset();
    m_graph_renderer.reset();
    m_background_renderer.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown renderering";
}

auto application::shutdown_commands() -> void
{
    m_cmds.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown commands";
}

auto application::shutdown_graph_interface() -> void
{
    m_graph_iface.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown graph interface";
}

/***********************************************************
 * GUI Prepare                                             *
 ***********************************************************/

auto application::prepare_weights_editor() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_weights_editor();

    editor.set_weights(
        [this](auto type) {
            return weights::get_weight(
                m_graph_iface->get_weights_backend(), type);
        });

    BOOST_LOG_TRIVIAL(debug) << "prepared weights editor";
}

auto application::prepare_layout_editor() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_layout_editor();

    editor.set_layout(
        [this]()
        { return layout::get_layout_id(m_graph_iface->get_layout_backend()); });

    editor.set_topology(
        [this]() {
            return layout::get_topology_id(m_graph_iface->get_layout_backend());
        });

    editor.set_scale(
        [this]()
        { return layout::get_scale(m_graph_iface->get_layout_backend()); });

    BOOST_LOG_TRIVIAL(debug) << "prepared layout editor";
}

auto application::prepare_scaling_editor() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_scaling_editor();

    editor.set_dims(
        [this](auto tag)
        {
            return scaling::get_factor_dims(
                m_graph_iface->get_scaling_backend(), tag);
        });

    editor.set_baseline(
        [this](auto tag)
        {
            return scaling::get_factor_baseline(
                m_graph_iface->get_scaling_backend(), tag);
        });

    editor.set_enabled(
        [this](auto tag)
        {
            return scaling::is_factor_enabled(
                m_graph_iface->get_scaling_backend(), tag);
        });

    editor.set_min_ratio(
        [this](auto tag)
        {
            return scaling::get_factor_min_ratio(
                m_graph_iface->get_scaling_backend(), tag);
        });

    editor.set_max_ratio(
        [this](auto tag)
        {
            return scaling::get_factor_max_ratio(
                m_graph_iface->get_scaling_backend(), tag);
        });

    BOOST_LOG_TRIVIAL(debug) << "prepared scaling editor";
}

auto application::prepare_degrees_editor() -> void
{
    assert(m_graph_renderer);

    auto& backend = m_graph_renderer->get_degrees_evaluator().backend();
    auto& frontend = m_gui->get_menu_bar().get_degrees_editor();

    frontend.set_in_light_threshold(
        [this, &backend]() {
            return rendering::get_in_degree_evaluation_light_threshold(backend);
        });

    frontend.set_out_light_threshold(
        [this, &backend]() {
            return rendering::get_out_degree_evaluation_light_threshold(
                backend);
        });

    frontend.set_in_medium_threshold(
        [this, &backend]() {
            return rendering::get_in_degree_evaluation_medium_threshold(
                backend);
        });

    frontend.set_out_medium_threshold(
        [this, &backend]() {
            return rendering::get_out_degree_evaluation_medium_threshold(
                backend);
        });

    frontend.set_in_heavy_threshold(
        [this, &backend]() {
            return rendering::get_in_degree_evaluation_heavy_threshold(backend);
        });

    frontend.set_out_heavy_threshold(
        [this, &backend]() {
            return rendering::get_out_degree_evaluation_heavy_threshold(
                backend);
        });

    frontend.set_in_light_particles([ this, &backend ]() -> const auto& {
        return rendering::get_in_degree_evaluation_light_effect(backend);
    });

    frontend.set_out_light_particles([ this, &backend ]() -> const auto& {
        return rendering::get_out_degree_evaluation_light_effect(backend);
    });

    frontend.set_in_medium_particles([ this, &backend ]() -> const auto& {
        return rendering::get_in_degree_evaluation_medium_effect(backend);
    });

    frontend.set_out_medium_particles([ this, &backend ]() -> const auto& {
        return rendering::get_out_degree_evaluation_medium_effect(backend);
    });

    frontend.set_in_heavy_particles([ this, &backend ]() -> const auto& {
        return rendering::get_in_degree_evaluation_heavy_effect(backend);
    });

    frontend.set_out_heavy_particles([ this, &backend ]() -> const auto& {
        return rendering::get_out_degree_evaluation_heavy_effect(backend);
    });

    frontend.set_in_applied(
        [this, &backend]()
        { return rendering::is_in_degree_evaluation_applied(backend); });

    frontend.set_out_applied(
        [this, &backend]()
        { return rendering::is_out_degree_evaluation_applied(backend); });

    BOOST_LOG_TRIVIAL(debug) << "prepared degrees editor";
}

auto application::prepare_clustering_editor() -> void
{
    assert(m_graph_iface);

    auto& backend = m_graph_iface->get_clustering_backend();
    auto& frontend = m_gui->get_menu_bar().get_clustering_editor();

    frontend.set_clusterer([this, &backend]()
                           { return clustering::get_clusterer_id(backend); });

    frontend.set_mst_finder([this, &backend]()
                            { return clustering::get_mst_finder_id(backend); });

    frontend.set_k([this, &backend]() { return clustering::get_k(backend); });

    frontend.set_snn_thres([this, &backend]()
                           { return clustering::get_snn_threshold(backend); });

    frontend.set_min_modularity(
        [this, &backend]() { return clustering::get_min_modularity(backend); });

    frontend.set_llp_gamma([this, &backend]()
                           { return clustering::get_llp_gamma(backend); });

    frontend.set_llp_steps([this, &backend]()
                           { return clustering::get_llp_steps(backend); });

    BOOST_LOG_TRIVIAL(debug) << "prepared clustering editor";
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

auto application::prepare_background_configurator() -> void
{
    assert(m_background_renderer);

    auto& bkg_gui = m_gui->get_menu_bar().get_bkg_configurator();
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

auto application::prepare_graph_configurator() -> void
{
    assert(m_graph_renderer);

    auto& graph_gui = m_gui->get_menu_bar().get_graph_configurator();
    const auto& cfg = m_graph_renderer->config_data();

    graph_gui.set_node_mesh(cfg.vertex_mesh);
    graph_gui.set_node_material(cfg.vertex_material);
    graph_gui.set_node_scale(to_scale(cfg.vertex_scale));
    graph_gui.set_node_font(cfg.vertex_id_font_name);
    graph_gui.set_node_font_color(to_rgba(cfg.vertex_id_color));
    graph_gui.set_node_char_height(cfg.vertex_id_char_height);
    graph_gui.set_node_space_width(cfg.vertex_id_space_width);
    graph_gui.set_edge_material(cfg.edge_material);
    graph_gui.set_edge_tip_mesh(cfg.edge_tip_mesh);
    graph_gui.set_edge_tip_material(cfg.edge_tip_material);
    graph_gui.set_edge_tip_scale(to_scale(cfg.edge_tip_scale));
    graph_gui.set_edge_font(cfg.edge_type_font_name);
    graph_gui.set_edge_font_color(to_rgba(cfg.edge_type_color));
    graph_gui.set_edge_char_height(cfg.edge_type_char_height);
    graph_gui.set_edge_space_width(cfg.edge_type_space_width);

    BOOST_LOG_TRIVIAL(debug) << "prepared graph configurator";
}

auto application::prepare_minimap_configurator() -> void
{
    assert(m_minimap_renderer);

    auto& minimap_gui = m_gui->get_menu_bar().get_minimap_configurator();
    const auto& cfg = m_minimap_renderer->config_data();

    minimap_gui.set_left(cfg.left);
    minimap_gui.set_top(cfg.top);
    minimap_gui.set_right(cfg.right);
    minimap_gui.set_bottom(cfg.bottom);
    minimap_gui.set_background_color(to_rgba(cfg.background_col));
    minimap_gui.set_zoom_out(cfg.zoom_out);
    minimap_gui.set_render_shadows(cfg.render_shadows);
    minimap_gui.set_render_sky(cfg.render_sky);
    minimap_gui.set_render_vertices(cfg.render_vertices);
    minimap_gui.set_render_vertex_ids(cfg.render_vertex_ids);
    minimap_gui.set_render_edges(cfg.render_edges);
    minimap_gui.set_render_edge_types(cfg.render_edge_types);
    minimap_gui.set_render_edge_tips(cfg.render_edge_tips);
    minimap_gui.set_render_particles(cfg.render_particles);

    BOOST_LOG_TRIVIAL(debug) << "prepared minimap configurator";
}

auto application::prepare_gui_configurator() -> void
{
    auto& gui_cfg = m_gui->get_menu_bar().get_gui_configurator();
    const auto& cfg = gui::get_configs();

    gui_cfg.set_color_theme(cfg.color_theme);
    gui_cfg.set_frame_rounding(cfg.frame_rounding);
    gui_cfg.set_frame_bordered(cfg.frame_bordered);
    gui_cfg.set_window_bordered(cfg.window_bordered);
    gui_cfg.set_popup_bordered(cfg.popup_bordered);

    BOOST_LOG_TRIVIAL(debug) << "prepared gui configurator";
}

auto application::prepare_menu_bar() -> void
{
    auto& bar = m_gui->get_menu_bar();

    bar.num_vertices() = boost::num_vertices(m_graph_iface->get_graph());
    bar.num_edges() = boost::num_edges(m_graph_iface->get_graph());

    bar.set_can_undo([this]() { return m_cmds->can_undo(); });
    bar.set_can_redo([this]() { return m_cmds->can_redo(); });

    BOOST_LOG_TRIVIAL(debug) << "prepared menu bar";
}

/***********************************************************
 * Presentation Connections                                *
 ***********************************************************/

auto application::connect_weights_presentation() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_weights_editor();
    auto& backend = m_graph_iface->get_weights_backend();

    editor.connect_to_dependency(
        [this, &backend](auto type, auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected: " << w << " for: " << type;
            pres::update_dependency_weight(*m_cmds, backend, type, w);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected weights restore";
            pres::restore_weights(*m_cmds, backend);
        });

    backend.connect(
        [this](auto, auto)
        {
            m_graph_renderer->render_weights(pres::edge_weight(*m_graph_iface));
            BOOST_LOG_TRIVIAL(info) << "rendered edge weights";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected weights presentation";
}

auto application::connect_layout_presentation() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_layout_editor();
    auto& backend = m_graph_iface->get_layout_backend();

    editor.connect_to_layout(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout: " << selection;
            pres::update_layout(*m_cmds, backend, selection);
        });

    editor.connect_to_topology(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected topology: " << selection;
            pres::update_layout_topology(*m_cmds, backend, selection);
        });

    editor.connect_to_scale(
        [this, &backend](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scale: " << selection;
            pres::update_layout_scale(*m_cmds, backend, selection);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout restore";
            pres::restore_layout(*m_cmds, backend);
        });

    backend.connect_to_layout(
        [this](const auto&)
        {
            m_graph_renderer->render_layout(
                pres::vertex_position(*m_graph_iface));
            BOOST_LOG_TRIVIAL(info) << "rendered layout";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected layout presentation";
}

auto application::connect_scaling_presentation() -> void
{
    assert(m_graph_iface);

    auto& editor = m_gui->get_menu_bar().get_scaling_editor();
    auto& backend = m_graph_iface->get_scaling_backend();

    editor.connect_to_baseline(
        [this, &backend](auto tag, auto baseline)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected baseline: " << baseline << "for: " << tag;
            pres::update_scaling_factor_baseline(
                *m_cmds, backend, tag, baseline);
        });

    editor.connect_to_dims(
        [this, &backend](auto tag, const auto& dims)
        {
            BOOST_LOG_TRIVIAL(info) << "selected new dims for: " << tag;
            pres::update_scaling_factor_dims(*m_cmds, backend, tag, dims);
        });

    editor.connect_to_enabled(
        [this, &backend](auto tag, auto enabled)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected enabled: " << enabled << " for: " << tag;
            pres::update_scaling_factor_enablement(
                *m_cmds, backend, tag, enabled);
        });

    editor.connect_to_min_ratio(
        [this, &backend](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected min ratio: " << ratio << " for: " << tag;
            pres::update_scaling_factor_min_ratio(*m_cmds, backend, tag, ratio);
        });

    editor.connect_to_max_ratio(
        [this, &backend](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected max ratio: " << ratio << " for: " << tag;
            pres::update_scaling_factor_max_ratio(*m_cmds, backend, tag, ratio);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling restore";
            pres::restore_scaling(*m_cmds, backend);
        });

    backend.connect(
        [this](const auto&)
        {
            m_graph_renderer->render_scaling(
                pres::vertex_scale(*m_graph_iface));
            BOOST_LOG_TRIVIAL(info) << "rendered vertex scaling";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected scaling presentation";
}

auto application::connect_degrees_presentation() -> void
{
    assert(m_graph_renderer);

    auto& backend = m_graph_renderer->get_degrees_evaluator().backend();
    auto& editor = m_gui->get_menu_bar().get_degrees_editor();

    editor.connect_to_in_light_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees light threshold: " << thres;
            pres::update_in_degree_evaluation_light_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_in_medium_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees medium threshold: " << thres;
            pres::update_in_degree_evaluation_medium_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_in_heavy_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees heavy threshold: " << thres;
            pres::update_in_degree_evaluation_heavy_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_out_light_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees light threshold: " << thres;
            pres::update_out_degree_evaluation_light_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_out_medium_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees medium threshold: " << thres;
            pres::update_out_degree_evaluation_medium_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_out_heavy_threshold(
        [this, &backend](auto thres)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees heavy threshold: " << thres;
            pres::update_out_degree_evaluation_heavy_threshold(
                *m_cmds, backend, thres);
        });

    editor.connect_to_in_light_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees light particles: " << particles;
            pres::update_in_degree_evaluation_light_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_in_medium_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees medium particles: " << particles;
            pres::update_in_degree_evaluation_medium_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_in_heavy_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees heavy particles: " << particles;
            pres::update_in_degree_evaluation_heavy_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_out_light_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees light particles: " << particles;
            pres::update_out_degree_evaluation_light_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_out_medium_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees medium particles: " << particles;
            pres::update_out_degree_evaluation_medium_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_out_heavy_particles(
        [this, &backend](auto particles)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees heavy particles: " << particles;
            pres::update_out_degree_evaluation_heavy_particles(
                *m_cmds, backend, std::string(particles));
        });

    editor.connect_to_in_applied(
        [this, &backend](auto applied)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected in degrees applied: " << applied;
            pres::update_in_degree_evaluation_applied(
                *m_cmds, backend, applied);
        });

    editor.connect_to_out_applied(
        [this, &backend](auto applied)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected out degrees applied: " << applied;
            pres::update_out_degree_evaluation_applied(
                *m_cmds, backend, applied);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected in degrees restore";
            pres::restore_degrees(*m_cmds, backend);
        });

    backend.connect_to_in_degree_evaluation(
        [this](const auto&)
        {
            m_graph_renderer->render_in_degree_particles();
            BOOST_LOG_TRIVIAL(info) << "rendered out degree particles";
        });

    backend.connect_to_out_degree_evaluation(
        [this](const auto&)
        {
            m_graph_renderer->render_out_degree_particles();
            BOOST_LOG_TRIVIAL(info) << "rendered in degree particles";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected degrees presentation";
}

auto application::connect_clustering_presentation() -> void
{
    assert(m_graph_renderer);
    assert(m_graph_iface);

    auto& backend = m_graph_iface->get_clustering_backend();
    auto& editor = m_gui->get_menu_bar().get_clustering_editor();

    editor.connect_to_cluster(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected cluster";
            pres::update_clusters(*m_cmds, backend);
        });

    editor.connect_to_hide(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected hide clusters";
            m_graph_renderer->hide_clusters();
        });

    editor.connect_to_clusterer(
        [this, &backend](auto id)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clusterer: " << id;
            pres::update_clusterer(*m_cmds, backend, id);
        });

    editor.connect_to_mst_finder(
        [this, &backend](auto id)
        {
            BOOST_LOG_TRIVIAL(info) << "selected mst finder: " << id;
            pres::update_clustering_mst_finder(*m_cmds, backend, id);
        });

    editor.connect_to_k(
        [this, &backend](auto k)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering k: " << k;
            pres::update_clustering_k(*m_cmds, backend, k);
        });

    editor.connect_to_snn_thres(
        [this, &backend](auto t)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering snn t " << t;
            pres::update_clustering_snn_threshold(*m_cmds, backend, t);
        });

    editor.connect_to_min_modularity(
        [this, &backend](auto q)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering min q " << q;
            pres::update_clustering_min_modularity(*m_cmds, backend, q);
        });

    editor.connect_to_llp_gamma(
        [this, &backend](auto g)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering llp gamma " << g;
            pres::update_clustering_llp_gamma(*m_cmds, backend, g);
        });

    editor.connect_to_llp_steps(
        [this, &backend](auto s)
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering llp steps " << s;
            pres::update_clustering_llp_steps(*m_cmds, backend, s);
        });

    editor.connect_to_restore(
        [this, &backend]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected clustering restore";
            pres::restore_clustering(*m_cmds, backend);
        });

    backend.connect_to_clusters(
        [this](const auto&)
        {
            m_graph_renderer->render_clusters(
                pres::vertex_cluster(*m_graph_iface));
            BOOST_LOG_TRIVIAL(info) << "rendered clusters";
        });

    BOOST_LOG_TRIVIAL(debug) << "connected clustering presentation";
}

auto application::connect_background_presentation() -> void
{
    assert(m_background_renderer);

    auto& iface = m_gui->get_menu_bar().get_bkg_configurator();
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

auto application::connect_graph_presentation() -> void
{
    assert(m_graph_renderer);

    auto& iface = m_gui->get_menu_bar().get_graph_configurator();
    auto& api = m_graph_renderer->config_api();

    iface.connect_to_node_mesh(
        [this, &api](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node mesh: " << mesh;
            api.set_vertex_mesh(std::string(mesh));
        });

    iface.connect_to_node_material(
        [this, &api](auto material)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node material: " << material;
            api.set_vertex_material(std::string(material));
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
            api.set_edge_material(std::string(mat));
        });

    iface.connect_to_edge_tip_mesh(
        [this, &api](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge tip mesh: " << mesh;
            api.set_edge_tip_mesh(std::string(mesh));
        });

    iface.connect_to_edge_tip_material(
        [this, &api](auto mat)
        {
            BOOST_LOG_TRIVIAL(info) << "selected edge tip material: " << mat;
            api.set_edge_tip_material(std::string(mat));
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
            api.set_edge_type_font_name(std::string(font));
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

auto application::connect_minimap_presentation() -> void
{
    assert(m_gui);
    assert(m_minimap_renderer);

    auto& iface = m_gui->get_menu_bar().get_minimap_configurator();
    auto& api = m_minimap_renderer->config_api();

    iface.connect_to_left(
        [&api](auto c)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap left corner: " << c;
            api.set_left(c);
        });

    iface.connect_to_top(
        [&api](auto c)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap top corner: " << c;
            api.set_top(c);
        });

    iface.connect_to_right(
        [&api](auto c)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap right corner: " << c;
            api.set_right(c);
        });

    iface.connect_to_bottom(
        [&api](auto c)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap bottom corner: " << c;
            api.set_bottom(c);
        });

    iface.connect_to_background_color(
        [&api](const auto& col)
        {
            BOOST_LOG_TRIVIAL(info) << "selected new minimap background col";
            api.set_background_color(to_color(col));
        });

    iface.connect_to_zoom_out(
        [&api](auto z)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap zoom out: " << z;
            api.set_zoom_out(z);
        });

    iface.connect_to_render_shadows(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap render shadows: " << v;
            api.set_render_shadows(v);
        });

    iface.connect_to_render_sky(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap render sky: " << v;
            api.set_render_sky(v);
        });

    iface.connect_to_render_vertices(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected minimap render vertices: " << v;
            api.set_render_vertices(v);
        });

    iface.connect_to_render_vertex_ids(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected minimap render vertex ids: " << v;
            api.set_render_vertex_ids(v);
        });

    iface.connect_to_render_edges(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap render edges: " << v;
            api.set_render_edges(v);
        });

    iface.connect_to_render_edge_types(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected minimap render edge types: " << v;
            api.set_render_edge_types(v);
        });

    iface.connect_to_render_edge_tips(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected minimap render edge tips: " << v;
            api.set_render_edge_tips(v);
        });

    iface.connect_to_render_particles(
        [&api](auto v)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected minimap render particles: " << v;
            api.set_render_particles(v);
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap apply";
            ui::apply_configs(*m_minimap_renderer);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap preview";
            ui::begin_preview(*m_minimap_renderer);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap cancel";
            ui::end_preview(*m_minimap_renderer);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected minimap restore";
            ui::restore_defaults(*m_minimap_renderer);
            prepare_minimap_configurator();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected minimap presentation";
}

auto application::connect_gui_presentation() -> void
{
    auto& iface = m_gui->get_menu_bar().get_gui_configurator();
    auto& api = gui::get_config_api();

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
            gui::ui_adaptor adaptor;
            ui::apply_configs(adaptor);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui preview";
            gui::ui_adaptor adaptor;
            ui::begin_preview(adaptor);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui cancel";
            gui::ui_adaptor adaptor;
            ui::end_preview(adaptor);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui restore";
            gui::ui_adaptor adaptor;
            ui::restore_defaults(adaptor);
            prepare_gui_configurator();
        });

    BOOST_LOG_TRIVIAL(debug) << "connected gui presentation";
}

auto application::connect_menu_bar_presentation() -> void
{
    auto& bar = m_gui->get_menu_bar();

    bar.connect_to_quit(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected quit";
            getRoot()->queueEndRendering();
        });

    bar.connect_to_undo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected undo";
            m_cmds->undo();
        });

    bar.connect_to_redo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected redo";
            m_cmds->redo();
        });

    bar.connect_to_search(
        [this](const auto& id)
        {
            BOOST_LOG_TRIVIAL(info) << "searched for: " << id;
            pres::pan_at(*m_graph_iface, id, m_background_renderer->cam_node());
        });

    BOOST_LOG_TRIVIAL(debug) << "connected undo redo presentation";
}

} // namespace application