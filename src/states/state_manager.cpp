#include "state_manager.hpp"

#include "state.hpp"
#include "state_machine.hpp"

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

void state_manager::deregister_state(state_type type)
{
    m_state_map.erase(type);
}

void state_manager::clear_all_states() { m_state_map.clear(); }

} // namespace states