// Contains an interface for undoable/redoable general purpose actions.
// Soultatos Stefanos 2022.

#ifndef APPLICATION_COMMAND_HPP
#define APPLICATION_COMMAND_HPP

#include <memory>

namespace application
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

    virtual auto clone() const -> std::unique_ptr< command > = 0;
};

} // namespace application

#endif // APPLICATION_COMMAND_HPP
