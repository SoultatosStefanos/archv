// Contains an entry point to the view package.
// Soultatos Stefanos 2022

#ifndef VIEW_CORE_HPP
#define VIEW_CORE_HPP

#include "paused_state.hpp"
#include "running_state.hpp"
#include "state_event_dispatcher.hpp"
#include "state_machine.hpp"

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>

namespace view
{

class core
{
public:
    using vertices = running_state::vertices;

    explicit core(vertices ids, Ogre::RenderWindow& win)
    : m_dispatcher { m_sm }
    , m_running { std::move(ids), win, m_sm }
    , m_paused { win, m_sm }
    {
        m_sm.start(&m_running);
    }

    auto get_event_dispatcher() const -> const auto& { return m_dispatcher; }
    auto get_event_dispatcher() -> auto& { return m_dispatcher; }

    auto get_running_state() const -> const auto& { return m_running; }
    auto get_running_state() -> auto& { return m_running; }

    auto get_paused_state() const -> const auto& { return m_paused; }
    auto get_paused_state() -> auto& { return m_paused; }

private:
    state_machine m_sm;
    state_event_dispatcher m_dispatcher;
    running_state m_running;
    paused_state m_paused;
};

} // namespace view

#endif // VIEW_CORE_HPP
