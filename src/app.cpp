#include "app.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>

// TODO Get hardcoded data from config

app::app(const std::string& name) : base(name) { }

auto app::frameStarted(const Ogre::FrameEvent& evt) -> bool
{
    base::frameStarted(evt);

    if (get_state_machine().has_active_state())
        get_state_machine().get_active_state()->frameStarted(evt);

    return true;
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

    get_state_machine().start(&get_graph_visualization());
    lay_graph(get_layout_core().get_layout());
}

void app::setup_architecture()
{
    const auto& root = config::json_archive::get().at(ARCHV_INPUT_GRAPH_PATH);

    m_st = architecture::deserialize_symbols(root);
    auto&& [g, _] = architecture::deserialize_dependencies(root, m_st);
    m_g = std::move(g);

    BOOST_LOG_TRIVIAL(info) << "setup architecture";
}

void app::setup_commands()
{
    m_cmds = std::make_unique< command_history >();

    BOOST_LOG_TRIVIAL(info) << "setup commands";
}

void app::setup_dependencies()
{
    auto deps_table
        = dependencies::core::hash_table { { "Inherit", 1 },
                                           { "Friend", 1 },
                                           { "NestedClass", 1 },
                                           { "ClassField", 1 },
                                           { "ClassTemplateParent", 1 },
                                           { "ClassTemplateArg", 1 },
                                           { "MethodReturn", 1 },
                                           { "MethodArg", 1 },
                                           { "MethodDefinition", 1 },
                                           { "MemberExpr", 1 },
                                           { "MethodTemplateArgs", 1 } };

    m_dependencies = std::make_unique< dependencies_core >(
        get_cmds(), std::move(deps_table));

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
        std::move(weight_map),
        "Gursoy Atun",
        "Sphere",
        100);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

void app::setup_view()
{
    m_sm = std::make_unique< state_machine >();

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

    m_pause_menu = std::make_unique< pause_menu >(get_state_machine());

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
