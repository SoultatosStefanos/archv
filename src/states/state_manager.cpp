#include "state_manager.hpp"

#include "state.hpp"
#include "state_machine.hpp"

#include <OGRE/Ogre.h>
#include <cassert>

namespace states
{

auto state_manager::get_state(state_type type) const -> state*
{
    assert(m_state_map.contains(type));
    return m_state_map.at(type).get();
}

auto state_manager::get_initial_state() const -> state*
{
    static constexpr auto initial_state_type = state_type::intro;

    assert(m_state_map.contains(initial_state_type));
    return m_state_map.at(initial_state_type).get();
}

void state_manager::prepare_state(state_type, Ogre::Root&, state_machine&)
{
    // TODO
}

void state_manager::prepare_all_states(Ogre::Root& root, state_machine& sm)
{
    prepare_state(state_type::intro, root, sm);
    prepare_state(state_type::menu, root, sm);
    prepare_state(state_type::running, root, sm);
    prepare_state(state_type::paused, root, sm);
}

void state_manager::reset_all_states() { m_state_map.clear(); }

} // namespace states