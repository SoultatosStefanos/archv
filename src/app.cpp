#include "app.hpp"

#include <OGRE/OgreRoot.h>
#include <boost/log/trivial.hpp>

// TODO Get hardcoded data from config

app::app(const std::string& name) : base(name) { }

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
        boost::get(boost::edge_bundle, get_graph()));

    m_layout = std::make_unique< layout_core >(
        get_cmds(),
        get_graph(),
        std::move(weight_map),
        "Gursoy Atun",
        "Sphere",
        100);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();
    base::shutdown();
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
