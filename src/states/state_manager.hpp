// Contains a class responsible for managing the lifetime of the concrete
// states.
// Soultatos Stefanos 2022

#ifndef STATES_STATE_MANAGER_HPP
#define STATES_STATE_MANAGER_HPP

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>

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

    template < typename State, typename... Args >
    requires std::derived_from< State, state >
    void register_state(state_type type, Args&&... args)
    {
        if (!m_state_map.contains(type))
            m_state_map[type]
                = std::make_unique< State >(std::forward< Args >(args)...);
    }

    void deregister_state(state_type type);

    void clear_all_states();

private:
    using state_map
        = std::unordered_map< state_type, std::unique_ptr< state > >;

    state_map m_state_map;
};

} // namespace states

#endif // STATES_STATE_MANAGER_HPP
