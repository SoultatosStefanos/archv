// Contains general purpose undo/redo utilites.
// Soultatos Stefanos 2022

#ifndef UTILITY_UNDO_REDO_HPP
#define UTILITY_UNDO_REDO_HPP

#include <list>
#include <memory>

namespace utility
{

class command
{
public:
    command() = default;
    command(const command&) = default;
    command(command&&) = default;
    virtual ~command() = default;

    auto operator=(const command&) -> command& = default;
    auto operator=(command&&) -> command& = default;

    virtual void execute() = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;

    virtual auto clone() const -> std::unique_ptr<command> = 0;
};

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

} // namespace utility

#endif // UTILITY_UNDO_REDO_HPP
