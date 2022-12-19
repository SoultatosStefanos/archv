// Contains a class representing the history of all of the executed commands.
// Soultatos Stefanos 2022

#ifndef UNDO_REDO_COMMAND_HISTORY_HPP
#define UNDO_REDO_COMMAND_HISTORY_HPP

#include <list>   // for list
#include <memory> // for unique_ptr

namespace undo_redo
{

class command;

class command_history
{
public:
    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    auto next_undo() const -> const command*;
    auto next_redo() const -> const command*;

    auto execute(std::unique_ptr< command > cmd) -> void;

    auto undo() -> void;
    auto redo() -> void;

private:
    using cache = std::list< std::unique_ptr< command > >;
    using cache_iterator = cache::iterator;

    cache m_history;
    cache_iterator m_i { std::end(m_history) };
};

} // namespace undo_redo

#endif // UNDO_REDO_COMMAND_HISTORY_HPP
