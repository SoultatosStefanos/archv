#include "command_history.hpp"

#include "command.hpp"

#include <cassert>

namespace undo_redo
{

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
    if (m_history.empty() or m_i == std::begin(m_history))
        return;
    else
        (*--m_i)->undo();
}

void command_history::redo()
{
    if (m_history.empty() or m_i == std::end(m_history))
        return;
    else
        (*m_i++)->redo();
}

} // namespace undo_redo