#include "app.hpp"

#include <OGRE/Ogre.h>
#include <SDL2/SDL_mouse.h>
#include <cassert>
#include <memory>

using namespace Ogre;
using namespace OgreBites;
using namespace architecture;

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

app::app(const graph& g) : ApplicationContext("ARCHV"), m_g { g } { }

void app::setup()
{
    ApplicationContext::setup();
    setup_resources();
    setup_states();
    setup_command_history();
    setup_layout();
}

void app::setup_resources()
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void app::setup_states()
{
    states::rendering_state::vertices ids;
    std::transform(
        boost::vertices(m_g).first,
        boost::vertices(m_g).second,
        std::back_inserter(ids),
        [this](auto v) { return m_g[v]; });

    m_sm = std::make_unique< states::state_machine >();
    m_paused = std::make_unique< states::paused_state >(
        *getRoot(), *getRenderWindow(), *m_sm);
    m_rendering = std::make_unique< states::rendering_state >(
        ids, *getRoot(), *getRenderWindow(), *m_sm, m_paused.get());

    m_dispatcher = std::make_unique< states::state_event_dispatcher >(*m_sm);
    addInputListener(m_dispatcher.get());

    m_sm->start(m_rendering.get());
}

void app::setup_command_history()
{
    m_cmds = std::make_unique< utility::command_history >();
}

void app::setup_layout()
{
    m_layout_sys = std::make_unique< layout::core< graph, weight_map > >(
        *m_cmds,
        m_g,
        weight_map(1),
        "Gursoy Atun", // TODO Config
        "Sphere",      // TODO Config
        100);          // TODO Config

    m_layout_sys->connect_to_layout(
        [this](const auto& l)
        {
            for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
                m_rendering->position_vertex(m_g[v], l.x(v), l.y(v), l.z(v));
        });

    const auto& l = m_layout_sys->get_layout();
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        m_rendering->position_vertex(m_g[v], l.x(v), l.y(v), l.z(v));
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_layout();
    shutdown_command_history();
    shutdown_states();
    shutdown_resources();
    ApplicationContext::shutdown();
}

void app::shutdown_layout() { m_layout_sys.reset(); }

void app::shutdown_command_history() { m_cmds.reset(); }

void app::shutdown_states()
{
    removeInputListener(m_dispatcher.get());
    m_dispatcher.reset();

    m_sm.reset();
    m_rendering.reset();
    m_paused.get();
}

void app::shutdown_resources()
{
    ResourceGroupManager::getSingleton().shutdownAll();
}