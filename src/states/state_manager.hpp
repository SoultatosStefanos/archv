// Contains a class responsible for managing the lifetime of the concrete
// states.
// Soultatos Stefanos 2022

#ifndef STATES_STATE_MANAGER_HPP
#define STATES_STATE_MANAGER_HPP

#include <concepts>
#include <memory>
#include <unordered_map>

namespace Ogre
{
class Root;
}

namespace states
{

class state;
class state_machine;

class state_manager
{
public:
    enum class state_type
    {
        intro,
        menu,
        running,
        paused
    };

    auto get_state(state_type type) const -> state*;
    auto get_initial_state() const -> state*;

    void prepare_state(state_type type, Ogre::Root& root, state_machine& sm);
    void prepare_all_states(Ogre::Root& root, state_machine& sm);

    void reset_all_states();

private:
    using state_map
        = std::unordered_map< state_type, std::unique_ptr< state > >;

    template < typename State >
    requires std::derived_from< State, state >
        && std::constructible_from< State, Ogre::Root&, state_machine& >
    void store_state(state_type type, Ogre::Root& root, state_machine& sm)
    {
        if (!m_state_map.contains(type))
            m_state_map[type] = std::make_unique< State >(root, sm);
    }

    state_map m_state_map;
};

} // namespace states

#endif // STATES_STATE_MANAGER_HPP
