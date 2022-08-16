#include "app.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>

app::app(int argc, const char* argv[]) : base("ARCHV")
{
    using namespace config;
    using namespace dependencies;
    using namespace layout;
    using json::archive;

    if (argc != 2)
        throw std::runtime_error("usage: ./<exec> <path/to/main/config.json>");

    m_main_config = deserialize_config(archive::get().at(argv[1]));

    m_dependencies_config = deserialize_dependencies(
        archive::get().at(m_main_config.dependencies_path));

    m_layout_config
        = deserialize_layout(archive::get().at(m_main_config.layout_path));
}

// NOTE Must let base handle frame first

auto app::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    assert(m_frames);
    base::frameStarted(e);
    return m_frames->frameStarted(e);
}

auto app::frameRenderingQueued(const Ogre::FrameEvent& e) -> bool
{
    assert(m_frames);
    base::frameRenderingQueued(e);
    return m_frames->frameRenderingQueued(e);
}

auto app::frameEnded(const Ogre::FrameEvent& e) -> bool
{
    assert(m_frames);
    base::frameEnded(e);
    return m_frames->frameEnded(e);
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
    setup_view();
    setup_gui();
    setup_rendering();

    connect_gui_with_dependencies();
    connect_gui_with_layout();
    connect_layout_with_rendering();
    connect_gui_with_command_history();

    get_state_machine().start(&get_graph_visualization());
    lay_graph(get_layout_core().get_layout());
}

void app::setup_architecture()
{
    const auto& root = json::archive::get().at(get_main_config().graph_path);

    m_st = architecture::deserialize_symbols(root);
    std::tie(m_g, std::ignore)
        = architecture::deserialize_dependencies(root, m_st);

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

void app::setup_view()
{
    m_sm = std::make_unique< state_machine >();

    m_frames = std::make_unique< state_frame_dispatcher >(*m_sm);

    m_input = std::make_unique< state_input_dispatcher >(*m_sm);
    addInputListener(m_input.get());

    BOOST_LOG_TRIVIAL(info) << "setup view";
}

void app::setup_gui()
{
    auto imgui = new Ogre::ImGuiOverlay();
    imgui->show();
    assert(imgui->isInitialised());

    Ogre::OverlayManager::getSingleton().addOverlay(imgui);

    m_pause_menu = std::make_unique< pause_menu >(
        get_state_machine(),
        get_dependencies_config(),
        get_layout_config().layouts,
        get_layout_config().topologies,
        get_layout_config().scales);

    // Default selections

    get_pause_menu().set_layout(get_layout_config().default_layout);
    get_pause_menu().set_topology(get_layout_config().default_topology);
    get_pause_menu().set_scale(get_layout_config().default_scale);

    BOOST_LOG_TRIVIAL(info) << "setup gui";
}

void app::setup_rendering()
{
    graph_visualization::vertex_ids ids;
    std::transform(
        boost::vertices(get_graph()).first,
        boost::vertices(get_graph()).second,
        std::back_inserter(ids),
        [this](auto v) { return get_graph()[v]; });

    m_graph_visualization = std::make_unique< graph_visualization >(
        get_state_machine(),
        &get_pause_menu(),
        *getRenderWindow(),
        std::move(ids));

    BOOST_LOG_TRIVIAL(info) << "setup rendering";
}

void app::connect_gui_with_dependencies()
{
    get_pause_menu().connect_to_dependency(
        [this](const auto& dependency, auto weight)
        { get_dependencies_core().update_weight(dependency, weight); });

    get_pause_menu().connect_to_dependencies_restore(
        [this]() { get_dependencies_core().revert_to_defaults(); });

    get_dependencies_core().connect(
        [this](const auto& dependency, auto weight)
        { get_pause_menu().set_dependency(dependency, weight); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

void app::connect_gui_with_layout()
{
    get_pause_menu().connect_to_layout(
        [this](const auto& selection)
        { get_layout_core().update_layout(selection); });

    get_pause_menu().connect_to_topology(
        [this](const auto& selection)
        { get_layout_core().update_topology(selection); });

    get_pause_menu().connect_to_scale(
        [this](auto selection)
        { get_layout_core().update_topology(selection); });

    get_pause_menu().connect_to_layout_restore(
        [this]() { get_layout_core().revert_to_defaults(); });

    get_layout_core().connect_to_layout(
        [this](const auto& layout)
        { get_pause_menu().set_layout(layout.desc()); });

    get_layout_core().connect_to_topology(
        [this](const auto& topology)
        {
            get_pause_menu().set_topology(topology.desc());
            get_pause_menu().set_scale(topology.scale());
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
    get_pause_menu().set_undo_enabled([this]()
                                      { return get_cmds().can_undo(); });

    get_pause_menu().set_redo_enabled([this]()
                                      { return get_cmds().can_redo(); });

    get_pause_menu().connect_to_undo([this]() { get_cmds().undo(); });
    get_pause_menu().connect_to_redo([this]() { get_cmds().redo(); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_rendering();
    shutdown_gui();
    shutdown_view();
    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();
    base::shutdown();
}

void app::shutdown_rendering()
{
    m_graph_visualization.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown rendering";
}

void app::shutdown_gui()
{
    m_pause_menu.reset();

    // named by Ogre
    Ogre::OverlayManager::getSingleton().destroy("ImGuiOverlay");

    BOOST_LOG_TRIVIAL(info) << "shutdown gui";
}

void app::shutdown_view()
{
    removeInputListener(m_input.get());
    m_input.reset();

    m_sm.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown view";
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

auto app::get_graph_visualization() const -> const graph_visualization&
{
    assert(m_graph_visualization);
    return *m_graph_visualization;
}

auto app::get_graph_visualization() -> graph_visualization&
{
    assert(m_graph_visualization);
    return *m_graph_visualization;
}

auto app::get_pause_menu() const -> const pause_menu&
{
    assert(m_pause_menu);
    return *m_pause_menu;
}

auto app::get_pause_menu() -> pause_menu&
{
    assert(m_pause_menu);
    return *m_pause_menu;
}

void app::lay_graph(const layout::layout< graph >& l)
{
    for (auto v : boost::make_iterator_range(boost::vertices(get_graph())))
        get_graph_visualization().lay_vertex(
            get_graph()[v], l.x(v), l.y(v), l.z(v));
}
