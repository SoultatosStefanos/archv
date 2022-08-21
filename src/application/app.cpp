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

    // TODO read only from ui
    m_main_config = deserialize(json::archive::get().at(argv[1]));

    m_dependencies_config = dependencies::deserialize(
        json::archive::get().at(m_main_config.dependencies_path));

    m_layout_config = layout::deserialize(
        json::archive::get().at(m_main_config.layout_path));
}

/***********************************************************
 * Events                                                  *
 ***********************************************************/

// NOTE Must let base handle frame first

auto app::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    base::frameStarted(e);

    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->frameStarted(e)
        : false;
}

auto app::frameRenderingQueued(const Ogre::FrameEvent& e) -> bool
{
    base::frameRenderingQueued(e);

    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->frameRenderingQueued(e)
        : false;
}

auto app::frameEnded(const Ogre::FrameEvent& e) -> bool
{
    base::frameEnded(e);

    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->frameEnded(e)
        : false;
}

auto app::frameRendered(const Ogre::FrameEvent& e) -> void
{
    if (get_state_machine().has_active_state())
        get_state_machine().get_active_state()->frameRendered(e);
}

auto app::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->keyPressed(e)
        : false;
}

auto app::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->keyReleased(e)
        : false;
}

auto app::touchMoved(const OgreBites::TouchFingerEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->touchMoved(e)
        : false;
}

auto app::touchPressed(const OgreBites::TouchFingerEvent& e) -> bool
{

    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->touchPressed(e)
        : false;
}

auto app::touchReleased(const OgreBites::TouchFingerEvent& e) -> bool
{

    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->touchReleased(e)
        : false;
}

auto app::mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->mouseMoved(e)
        : false;
}

auto app::mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->mouseWheelRolled(e)
        : false;
}

auto app::mousePressed(const OgreBites::MouseButtonEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->mousePressed(e)
        : false;
}

auto app::mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->mouseReleased(e)
        : false;
}

auto app::textInput(const OgreBites::TextInputEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->textInput(e)
        : false;
}

auto app::buttonPressed(const OgreBites::ButtonEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->buttonPressed(e)
        : false;
}

auto app::buttonReleased(const OgreBites::ButtonEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->buttonReleased(e)
        : false;
}

auto app::axisMoved(const OgreBites::AxisEvent& e) -> bool
{
    return get_state_machine().has_active_state()
        ? get_state_machine().get_active_state()->axisMoved(e)
        : false;
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
    setup_fsm();
    setup_gui();
    setup_rendering();

    connect_gui_with_dependencies();
    connect_gui_with_layout();
    connect_layout_with_rendering();
    connect_gui_with_command_history();

    get_state_machine().start(&get_visualization_state());
    lay_graph(get_layout_core().get_layout());

    addInputListener(this);
}

// FIXME
#define ARCHV_GRAPH_PATH "/home/stef/Documents/Projects/archv/build/graph.json"

void app::setup_architecture()
{
    const auto& root = json::archive::get().at(ARCHV_GRAPH_PATH);

    std::tie(m_st, m_g, std::ignore) = architecture::generate_arch(root);

    BOOST_LOG_TRIVIAL(info) << "setup architecture";
}

void app::setup_commands()
{
    m_cmds = std::make_unique< command_history >();

    BOOST_LOG_TRIVIAL(info) << "setup commands";
}

void app::setup_dependencies()
{
    m_dependencies = std::make_unique< dependencies_core >(
        get_cmds(), get_dependencies_config());

    BOOST_LOG_TRIVIAL(info) << "setup dependencies";
}

void app::setup_layout()
{
    auto weight_map = dependencies::make_dynamic_weight_map< graph >(
        get_dependencies_core().get_repo(),
        boost::get(boost::edge_bundle, std::as_const(get_graph())));

    m_layout = std::make_unique< layout_core >(
        get_cmds(),
        get_graph(),
        weight_map,
        get_layout_config().default_layout,
        get_layout_config().default_topology,
        get_layout_config().default_scale);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

void app::setup_fsm()
{
    m_sm = std::make_unique< state_machine >();

    BOOST_LOG_TRIVIAL(info) << "setup fsm";
}

void app::setup_gui()
{
    auto imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui);

    static_assert(std::is_convertible_v<
                  dependencies_config,
                  gui::menu_window::dependency_options >);

    static_assert(std::is_convertible_v<
                  layout_config::layout_options,
                  gui::menu_window::layout_options >);

    static_assert(std::is_convertible_v<
                  layout_config::topology_options,
                  gui::menu_window::topology_options >);

    static_assert(std::is_convertible_v<
                  layout_config::scale_options,
                  gui::menu_window::scale_options >);

    m_overlays = std::make_unique< overlay_manager >();

    auto menu_window = std::make_unique< gui::menu_window >(
        get_dependencies_config(),
        get_layout_config().layouts,
        get_layout_config().topologies,
        get_layout_config().scales);

    auto menu_bar = std::make_unique< gui::menu_bar >(
        [this]() { return get_cmds().can_undo(); },
        [this]() { return get_cmds().can_redo(); });

    m_paused = std::make_unique< paused_state >(
        get_state_machine(),
        get_overlay_manager(),
        std::move(menu_window),
        std::move(menu_bar));

    // Default selections

    get_paused_state().get_menu_window().set_layout(
        get_layout_config().default_layout);

    get_paused_state().get_menu_window().set_topology(
        get_layout_config().default_topology);

    get_paused_state().get_menu_window().set_scale(
        get_layout_config().default_scale);

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

void app::setup_rendering()
{
    rendering::graph_renderer::vertices ids;
    std::transform(
        boost::vertices(get_graph()).first,
        boost::vertices(get_graph()).second,
        std::back_inserter(ids),
        [this](auto v) { return get_graph()[v]; });

    auto renderer
        = rendering::graph_renderer(*getRenderWindow(), std::move(ids));

    m_visualization = std::make_unique< visualization_state >(
        get_state_machine(),
        get_overlay_manager(),
        &get_paused_state(),
        std::move(renderer));

    BOOST_LOG_TRIVIAL(info) << "setup rendering";
}

void app::connect_gui_with_dependencies()
{
    get_paused_state().get_menu_window().connect_to_dependency(
        [this](const auto& dependency, auto weight)
        { get_dependencies_core().update_weight(dependency, weight); });

    get_paused_state().get_menu_window().connect_to_dependencies_restore(
        [this]() { get_dependencies_core().revert_to_defaults(); });

    get_dependencies_core().connect(
        [this](const auto& dependency, auto weight) {
            get_paused_state().get_menu_window().set_dependency(
                dependency, weight);
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

void app::connect_gui_with_layout()
{
    get_paused_state().get_menu_window().connect_to_layout(
        [this](const auto& selection)
        { get_layout_core().update_layout(selection); });

    get_paused_state().get_menu_window().connect_to_topology(
        [this](const auto& selection)
        { get_layout_core().update_topology(selection); });

    get_paused_state().get_menu_window().connect_to_scale(
        [this](auto selection)
        { get_layout_core().update_topology(selection); });

    get_paused_state().get_menu_window().connect_to_layout_restore(
        [this]() { get_layout_core().revert_to_defaults(); });

    get_layout_core().connect_to_layout(
        [this](const auto& layout)
        { get_paused_state().get_menu_window().set_layout(layout.desc()); });

    get_layout_core().connect_to_topology(
        [this](const auto& topology)
        {
            get_paused_state().get_menu_window().set_topology(topology.desc());
            get_paused_state().get_menu_window().set_scale(topology.scale());
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with layout management";
}

void app::connect_layout_with_rendering()
{
    get_layout_core().connect_to_layout([this](const auto& l)
                                        { lay_graph(l); });

    BOOST_LOG_TRIVIAL(info) << "connected layout management with rendering";
}

void app::connect_gui_with_command_history()
{
    get_paused_state().get_menu_bar().connect_to_undo([this]()
                                                      { get_cmds().undo(); });
    get_paused_state().get_menu_bar().connect_to_redo([this]()
                                                      { get_cmds().redo(); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_rendering();
    shutdown_gui();
    shutdown_fsm();
    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();
    base::shutdown();
}

void app::shutdown_rendering()
{
    m_visualization.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown rendering";
}

void app::shutdown_gui()
{
    m_paused.reset();
    m_overlays.reset();

    // named by Ogre
    Ogre::OverlayManager::getSingleton().destroy("ImGuiOverlay");

    BOOST_LOG_TRIVIAL(info) << "shutdown gui";
}

void app::shutdown_fsm()
{
    m_sm.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown fsm";
}

void app::shutdown_layout()
{
    m_layout.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown layout";
}

void app::shutdown_dependencies()
{
    m_dependencies.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown dependencies";
}

void app::shutdown_commands()
{
    m_cmds.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown commands";
}

void app::shutdown_architecture()
{
    BOOST_LOG_TRIVIAL(info) << "shutdown architecture";
}

/***********************************************************
 * Main loop                                               *
 ***********************************************************/

void app::go() { getRoot()->startRendering(); }

/***********************************************************
 * Accessors                                               *
 ***********************************************************/

auto app::get_main_config() const -> const main_config&
{
    return m_main_config;
}

auto app::get_main_config() -> main_config& { return m_main_config; }

auto app::get_dependencies_config() const -> const dependencies_config&
{
    return m_dependencies_config;
}

auto app::get_dependencies_config() -> dependencies_config&
{
    return m_dependencies_config;
}

auto app::get_layout_config() const -> const layout_config&
{
    return m_layout_config;
}

auto app::get_layout_config() -> layout_config& { return m_layout_config; }

auto app::get_symbol_table() const -> const symbol_table& { return m_st; }

auto app::get_symbol_table() -> symbol_table& { return m_st; }

auto app::get_graph() const -> const graph& { return m_g; }

auto app::get_graph() -> graph& { return m_g; }

auto app::get_cmds() const -> const command_history&
{
    assert(m_cmds);
    return *m_cmds;
}

auto app::get_cmds() -> command_history&
{
    assert(m_cmds);
    return *m_cmds;
}

auto app::get_dependencies_core() const -> const dependencies_core&
{
    assert(m_dependencies);
    return *m_dependencies;
}

auto app::get_dependencies_core() -> dependencies_core&
{
    assert(m_dependencies);
    return *m_dependencies;
}

auto app::get_layout_core() const -> const layout_core&
{
    assert(m_layout);
    return *m_layout;
}

auto app::get_layout_core() -> layout_core&
{
    assert(m_layout);
    return *m_layout;
}

auto app::get_state_machine() const -> const state_machine&
{
    assert(m_sm);
    return *m_sm;
}

auto app::get_state_machine() -> state_machine&
{
    assert(m_sm);
    return *m_sm;
}

auto app::get_overlay_manager() const -> const overlay_manager&
{
    assert(m_overlays);
    return *m_overlays;
}

auto app::get_overlay_manager() -> overlay_manager&
{
    assert(m_overlays);
    return *m_overlays;
}

auto app::get_visualization_state() const -> const visualization_state&
{
    assert(m_visualization);
    return *m_visualization;
}

auto app::get_visualization_state() -> visualization_state&
{
    assert(m_visualization);
    return *m_visualization;
}

auto app::get_paused_state() const -> const paused_state&
{
    assert(m_paused);
    return *m_paused;
}

auto app::get_paused_state() -> paused_state&
{
    assert(m_paused);
    return *m_paused;
}

void app::lay_graph(const layout::layout< graph >& l)
{
    for (auto v : boost::make_iterator_range(boost::vertices(get_graph())))
        get_visualization_state().get_renderer().lay_vertex(
            get_graph()[v], l.x(v), l.y(v), l.z(v));
}

} // namespace application