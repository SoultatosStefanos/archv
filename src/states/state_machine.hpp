// Contains a class that is responsible for the inter-state transitions.
// Soultatos Stefanos 2022

#ifndef STATES_STATE_MACHINE_HPP
#define STATES_STATE_MACHINE_HPP

#include <stack>

namespace states
{

class state;

class state_machine
{
public:
    auto started() const -> bool { return m_started; }

    auto has_active_state() const -> bool;
    auto get_active_state() const -> state*;

    void start(state* s);
    void commit_transition_to(state* s);
    void transition_to(state* s);
    void fallback();

private:
    using state_frame = state*;
    using state_stack = std::stack< state_frame >;

    void push_state_frame(state_frame s);
    void pop_state_frame();

    state_stack m_state_stack;
    bool m_started { false };
};

} // namespace states

#endif // STATES_STATE_MACHINE_HPP
