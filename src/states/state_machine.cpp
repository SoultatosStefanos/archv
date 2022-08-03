#include "state_machine.hpp"

#include "state.hpp"

#include <cassert>

namespace states
{

auto state_machine::has_active_state() const -> bool
{
    return !m_state_stack.empty();
}

auto state_machine::get_active_state() const -> state*
{
    assert(m_state_stack.top());
    return m_state_stack.top();
}

void state_machine::start(state* s)
{
    assert(s);
    push_state_frame(s);
}

void state_machine::commit_transition_to(state* s)
{
    assert(s);

    if (has_active_state())
        pop_state_frame();

    push_state_frame(s);
}

void state_machine::transition_to(state* s)
{
    assert(s);

    if (has_active_state())
        get_active_state()->pause();

    push_state_frame(s);
}

void state_machine::fallback()
{
    if (has_active_state())
        pop_state_frame();

    if (has_active_state())
        get_active_state()->resume();
}

void state_machine::push_state_frame(state_frame s)
{
    assert(s);

    s->enter();
    m_state_stack.push(s);

    assert(has_active_state());
}

void state_machine::pop_state_frame()
{
    assert(!m_state_stack.empty());
    assert(m_state_stack.top());

    m_state_stack.top()->exit();
    m_state_stack.pop();
}

} // namespace states