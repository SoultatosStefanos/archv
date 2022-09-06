// Contains a class representing the history of all of the executed commands.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMAND_HISTORY_HPP
#define APPLICATION_COMMAND_HISTORY_HPP

#include <list>
#include <memory>

namespace application
{

class command;

class command_history
{
public:
    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    void execute(std::unique_ptr< command > cmd);

    void undo();
    void redo();

private:
    using cache = std::list< std::unique_ptr< command > >;
    using cache_iterator = cache::iterator;

    cache m_history;
    cache_iterator m_i { std::end(m_history) };
};

} // namespace application

#endif // APPLICATION_COMMAND_HISTORY_HPP
