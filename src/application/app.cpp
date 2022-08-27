#include "app.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>

namespace application
{

app::app(int argc, const char* argv[]) : base("ARCHV")
{
    if (argc != 2)
        throw std::runtime_error("usage: ./<exec> <path/to/main/config.json>");

    m_config = deserialize(json::archive::get().at(argv[1]));

    m_deps_config = dependencies::deserialize(
        json::archive::get().at(config().dependencies_path));

    m_layout_config
        = layout::deserialize(json::archive::get().at(config().layout_path));
}

/***********************************************************
 * Events                                                  *
 ***********************************************************/

auto app::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    base::frameStarted(e);
    multithreading::poll_message();
    Ogre::ImGuiOverlay::NewFrame();
    gui::overlay_manager::get().draw_all();
    return true;
}

auto app::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case OgreBites::SDLK_ESCAPE:
        getRoot()->queueEndRendering();
        break;

    case 'p':
        toggle_pause_resume();
        break;

    default:
        break;
    }

    return true;
}

auto app::pause() -> void
{
    assert(!paused());

    cameraman()->manualStop();
    removeInputListener(cameraman());

    gui::overlay_manager::get().submit(menu_bar());
    gui::overlay_manager::get().submit(menu_window());

    m_paused = true;

    assert(paused());
}

auto app::resume() -> void
{
    assert(paused());

    addInputListener(cameraman());

    gui::overlay_manager::get().withdraw(menu_bar());
    gui::overlay_manager::get().withdraw(menu_window());

    m_paused = false;

    assert(!paused());
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void app::setup()
{
    base::setup();
    setup_architecture();
    setup_commands();
    setup_dependencies();
    setup_layout();
    setup_rendering();
    setup_gui();
    setup_input();

    connect_gui_with_dependencies();
    connect_gui_with_layout();
    connect_gui_with_command_history();

    connect_rendering_with_layout();
}

auto app::id_map() const -> architecture::id_map
{
    return boost::get(boost::vertex_bundle, std::as_const(graph()));
}

auto app::dependency_map() const -> architecture::dependency_map
{
    return boost::get(boost::edge_bundle, std::as_const(graph()));
}

auto app::dynamic_weight_map() const -> weight_map
{
    return dependencies::make_dynamic_weight_map< architecture::graph >(
        dependencies_core()->get_repo(), dependency_map());
}

auto app::dynamic_position_map() const -> position_map
{
    return layout::make_dynamic_position_map(*layout_core());
}

// FIXME
#define ARCHV_GRAPH_PATH "/home/stef/Documents/Projects/archv/build/graph.json"

auto app::setup_architecture() -> void
{
    const auto& root = json::archive::get().at(ARCHV_GRAPH_PATH);

    std::tie(m_st, m_g, std::ignore) = architecture::generate_arch(root);

    BOOST_LOG_TRIVIAL(info) << "setup architecture";
}

auto app::setup_commands() -> void
{
    m_cmds = std::make_unique< undo_redo::command_history >();

    BOOST_LOG_TRIVIAL(info) << "setup commands";
}

auto app::setup_dependencies() -> void
{
    assert(cmd_history());

    m_deps = std::make_unique< dependencies::core >(
        *cmd_history(), dependencies_config());

    BOOST_LOG_TRIVIAL(info) << "setup dependencies";
}

auto app::setup_layout() -> void
{
    assert(cmd_history());
    assert(dependencies_core());

    m_layout
        = std::make_unique< layout::core< architecture::graph, weight_map > >(
            *cmd_history(),
            graph(),
            dynamic_weight_map(),
            layout_config().default_layout,
            layout_config().default_topology,
            layout_config().default_scale);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

auto app::setup_rendering() -> void
{
    using graph_renderer_type = rendering::graph_renderer<
        architecture::graph,
        architecture::id_map,
        weight_map,
        position_map >;

    m_bkgrd_renderer = std::make_unique< rendering::background_renderer >(
        *getRenderWindow());

    m_g_renderer = std::make_unique< graph_renderer_type >(
        &graph(),
        id_map(),
        dynamic_weight_map(),
        dynamic_position_map(),
        background_renderer()->scene(),
        *getRenderWindow());

    BOOST_LOG_TRIVIAL(info) << "setup rendering";
}

auto app::setup_gui() -> void
{
    assert(cmd_history());

    auto* imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui); // takes ownership

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    background_renderer()->scene()->addRenderQueueListener(ogre_overlay);

    static_assert(std::is_convertible_v<
                  dependencies::config_data,
                  gui::menu_window::dependency_options >);

    static_assert(std::is_convertible_v<
                  layout::config_data::layout_options,
                  gui::menu_window::layout_options >);

    static_assert(std::is_convertible_v<
                  layout::config_data::topology_options,
                  gui::menu_window::topology_options >);

    static_assert(std::is_convertible_v<
                  layout::config_data::scale_options,
                  gui::menu_window::scale_options >);

    m_menu_win = std::make_unique< gui::menu_window >(
        dependencies_config(),
        layout_config().layouts,
        layout_config().topologies,
        layout_config().scales);

    m_menu_bar = std::make_unique< gui::menu_bar >(
        [this]() { return cmd_history()->can_undo(); },
        [this]() { return cmd_history()->can_redo(); });

    // Default selections
    menu_window()->set_layout(layout_config().default_layout);
    menu_window()->set_topology(layout_config().default_topology);
    menu_window()->set_scale(layout_config().default_scale);

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

auto app::setup_input() -> void
{
    assert(background_renderer());

    m_cameraman = std::make_unique< OgreBites::CameraMan >(
        background_renderer()->cam_node());

    m_gui_input = std::make_unique< OgreBites::ImGuiInputListener >();

    addInputListener(cameraman());
    addInputListener(gui_input_handler());
    addInputListener(this);

    BOOST_LOG_TRIVIAL(info) << "setup input";
}

auto app::connect_gui_with_dependencies() -> void
{
    assert(menu_window());
    assert(dependencies_core());

    menu_window()->connect_to_dependency(
        [this](const auto& dependency, auto weight)
        { dependencies_core()->update_weight(dependency, weight); });

    menu_window()->connect_to_dependencies_restore(
        [this]() { dependencies_core()->revert_to_defaults(); });

    dependencies_core()->connect(
        [this](const auto& dependency, auto weight)
        { menu_window()->set_dependency(dependency, weight); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

auto app::connect_gui_with_layout() -> void
{
    assert(menu_window());
    assert(layout_core());

    menu_window()->connect_to_layout(
        [this](const auto& selection)
        { layout_core()->update_layout(selection); });

    menu_window()->connect_to_topology(
        [this](const auto& selection)
        { layout_core()->update_topology(selection); });

    menu_window()->connect_to_scale(
        [this](auto selection) { layout_core()->update_topology(selection); });

    menu_window()->connect_to_layout_restore(
        [this]() { layout_core()->revert_to_defaults(); });

    layout_core()->connect_to_layout(
        [this](const auto& layout)
        { menu_window()->set_layout(layout.desc()); });

    layout_core()->connect_to_topology(
        [this](const auto& topology)
        {
            menu_window()->set_topology(topology.desc());
            menu_window()->set_scale(topology.scale());
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with layout management";
}

auto app::connect_gui_with_command_history() -> void
{
    assert(menu_bar());
    assert(cmd_history());

    menu_bar()->connect_to_undo([this]() { cmd_history()->undo(); });
    menu_bar()->connect_to_redo([this]() { cmd_history()->redo(); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
}

auto app::connect_rendering_with_layout() -> void
{
    assert(graph_renderer());
    assert(layout_core());

    layout_core()->connect_to_layout(
        [this](const auto&)
        { graph_renderer()->set_vertex_pos(dynamic_position_map()); });
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown() -> void
{
    shutdown_input();
    shutdown_gui();
    shutdown_rendering();
    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();
    base::shutdown();
}

auto app::shutdown_input() -> void
{
    removeInputListener(gui_input_handler());
    removeInputListener(cameraman());

    m_gui_input.reset();
    m_cameraman.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown input";
}

auto app::shutdown_gui() -> void
{
    m_menu_bar.reset();
    m_menu_win.reset();

    auto* ogre_overlay = Ogre::OverlaySystem::getSingletonPtr();
    background_renderer()->scene()->removeRenderQueueListener(ogre_overlay);

    // named by Ogre
    Ogre::OverlayManager::getSingleton().destroy("ImGuiOverlay");

    BOOST_LOG_TRIVIAL(info) << "shutdown gui";
}

auto app::shutdown_rendering() -> void
{
    m_g_renderer.reset();
    m_bkgrd_renderer.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown rendering";
}

auto app::shutdown_layout() -> void
{
    m_layout.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown layout";
}

auto app::shutdown_dependencies() -> void
{
    m_deps.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown dependencies";
}

auto app::shutdown_commands() -> void
{
    m_cmds.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown commands";
}

auto app::shutdown_architecture() -> void
{
    BOOST_LOG_TRIVIAL(info) << "shutdown architecture";
}

/***********************************************************
 * Main loop                                               *
 ***********************************************************/

void app::go() { getRoot()->startRendering(); }

} // namespace application