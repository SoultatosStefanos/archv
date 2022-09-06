#include "command_history.hpp"

#include "command.hpp"

#include <cassert>

namespace application
{

auto command_history::can_undo() const -> bool
{
    return !m_history.empty() and m_i != std::begin(m_history);
}

auto command_history::can_redo() const -> bool
{
    return !m_history.empty() and m_i != std::end(m_history);
}

void command_history::execute(std::unique_ptr< command > cmd)
{
    assert(cmd);

    if (m_i != std::end(m_history))        // redos have been applied
        while (m_i != std::end(m_history)) // clear all redos
            m_i = m_history.erase(m_i);

    cmd->execute();
    m_history.push_back(std::move(cmd));

    assert(m_i == std::end(m_history));
}

void command_history::undo()
{
    if (!can_undo())
        return;
    else
        (*--m_i)->undo();
}

void command_history::redo()
{
    if (!can_redo())
        return;
    else
        (*m_i++)->redo();
}

} // namespace application