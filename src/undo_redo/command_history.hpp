// Contains a class representing the history of all of the executed commands.
// Soultatos Stefanos 2022

#ifndef UNDO_REDO_COMMAND_HISTORY_HPP
#define UNDO_REDO_COMMAND_HISTORY_HPP

#include <list>
#include <memory>

namespace undo_redo
{

class command;

class command_history
{
public:
    void execute(std::unique_ptr< command > cmd);

    void undo();
    void redo();

private:
    using cache = std::list< std::unique_ptr< command > >;
    using cache_iterator = cache::iterator;

    cache m_history;
    cache_iterator m_i { std::end(m_history) };
};

} // namespace undo_redo

#endif // UNDO_REDO_COMMAND_HISTORY_HPP
