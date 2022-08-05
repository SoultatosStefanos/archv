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
    setup_view();
}

void app::setup_architecture()
{
    const auto& root = config::json_archive::get().at(ARCHV_INPUT_GRAPH_PATH);

    auto st = architecture::deserialize_symbols(root);
    auto [g, _] = architecture::deserialize_dependencies(root, st);

    m_arch_manager
        = std::make_unique< arch_manager >(std::move(st), std::move(g));

    BOOST_LOG_TRIVIAL(info) << "setup architecture";
}

void app::setup_commands()
{
    m_cmd_manager = std::make_unique< command_manager >();

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

    m_deps_manager = std::make_unique< deps_manager >(
        get_cmd_manager(), std::move(deps_table));

    BOOST_LOG_TRIVIAL(info) << "setup dependencies";
}

void app::setup_layout()
{
    auto weight_map = dependencies::make_dynamic_weight_map< graph >(
        get_deps_manager().get_repo(),
        boost::get(boost::edge_bundle, get_arch_manager().get_graph()));

    m_layout_manager = std::make_unique< layout_manager >(
        get_cmd_manager(),
        get_arch_manager().get_graph(),
        std::move(weight_map),
        "Gursoy Atun",
        "Sphere",
        100);

    BOOST_LOG_TRIVIAL(info) << "setup layout";
}

void app::setup_view()
{
    auto view_model = view_manager::vertices();
    std::transform(
        boost::vertices(get_arch_manager().get_graph()).first,
        boost::vertices(get_arch_manager().get_graph()).second,
        std::back_inserter(view_model),
        [this](auto v) { return get_arch_manager().get_graph()[v]; });

    m_view_manager = std::make_unique< view_manager >(
        std::move(view_model), *getRenderWindow());

    get_layout_manager().connect_to_layout([this](const auto& l)
                                           { draw_layout(l); });

    draw_layout(get_layout_manager().get_layout());

    addInputListener(&get_view_manager().get_event_dispatcher());

    BOOST_LOG_TRIVIAL(info) << "setup view";
}

void app::draw_layout(const layout::layout< graph >& l)
{
    for (auto v : boost::make_iterator_range(
             boost::vertices(get_arch_manager().get_graph())))
    {
        get_view_manager().get_running_state().position_vertex(
            get_arch_manager().get_graph()[v], l.x(v), l.y(v), l.z(v));
    }
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_view();
    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();
    base::shutdown();
}

void app::shutdown_view()
{
    m_view_manager.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown view";
}

void app::shutdown_layout()
{
    m_layout_manager.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown layout";
}

void app::shutdown_dependencies()
{
    m_deps_manager.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown dependencies";
}

void app::shutdown_commands()
{
    m_cmd_manager.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown commands";
}

void app::shutdown_architecture()
{
    m_arch_manager.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown architecture";
}

/***********************************************************
 * Main loop                                               *
 ***********************************************************/

void app::go() { getRoot()->startRendering(); }

/***********************************************************
 * Accessors                                               *
 ***********************************************************/

auto app::get_view_manager() const -> const view_manager&
{
    assert(m_view_manager);
    return *m_view_manager;
}

auto app::get_view_manager() -> view_manager&
{
    assert(m_view_manager);
    return *m_view_manager;
}

auto app::get_cmd_manager() const -> const command_manager&
{
    assert(m_cmd_manager);
    return *m_cmd_manager;
}

auto app::get_cmd_manager() -> command_manager&
{
    assert(m_cmd_manager);
    return *m_cmd_manager;
}

auto app::get_arch_manager() const -> const arch_manager&
{
    assert(m_arch_manager);
    return *m_arch_manager;
}

auto app::get_arch_manager() -> arch_manager&
{
    assert(m_arch_manager);
    return *m_arch_manager;
}

auto app::get_deps_manager() const -> const deps_manager&
{
    assert(m_deps_manager);
    return *m_deps_manager;
}

auto app::get_deps_manager() -> deps_manager&
{
    assert(m_deps_manager);
    return *m_deps_manager;
}

auto app::get_layout_manager() const -> const layout_manager&
{
    assert(m_layout_manager);
    return *m_layout_manager;
}

auto app::get_layout_manager() -> layout_manager&
{
    assert(m_layout_manager);
    return *m_layout_manager;
}
