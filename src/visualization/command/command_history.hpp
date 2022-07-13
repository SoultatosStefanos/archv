// Contains a command history class, for the undo/redo feature.
// Soultatos Stefanos 2022

#ifndef COMMAND_COMMAND_HISTORY_HPP
#define COMMAND_COMMAND_HISTORY_HPP

#include "command.hpp"

#include <list>
#include <memory>

namespace visualization::command
{

class command_history
{
public:
    void execute(std::unique_ptr<command> cmd);

    void undo();
    void redo();

private:
    using cache = std::list<std::unique_ptr<command>>;
    using cache_iterator = cache::iterator;

    cache m_history;
    cache_iterator m_i{std::end(m_history)};
};

} // namespace visualization::command

#endif // COMMAND_COMMAND_HISTORY_HPP
