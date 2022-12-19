#include "command_history.hpp"

#include "command.hpp"

#include <cassert>

namespace undo_redo
{

auto command_history::can_undo() const -> bool
{
    return !m_history.empty() and m_i != std::cbegin(m_history);
}

auto command_history::can_redo() const -> bool
{
    return !m_history.empty() and m_i != std::cend(m_history);
}

auto command_history::next_undo() const -> const command*
{
    return m_i != std::cbegin(m_history) ? (*std::prev(m_i)).get() : nullptr;
}

auto command_history::next_redo() const -> const command*
{
    return m_i != std::cend(m_history) ? (*m_i).get() : nullptr;
}

auto command_history::execute(std::unique_ptr< command > cmd) -> void
{
    assert(cmd);

    if (m_i != std::end(m_history))        // redos have been applied
        while (m_i != std::end(m_history)) // clear all redos
            m_i = m_history.erase(m_i);

    cmd->execute();
    m_history.push_back(std::move(cmd));

    assert(m_i == std::end(m_history));
}

auto command_history::undo() -> void
{
    if (!can_undo())
        return;
    else
        (*--m_i)->undo();
}

auto command_history::redo() -> void
{
    if (!can_redo())
        return;
    else
        (*m_i++)->redo();
}

} // namespace undo_redo