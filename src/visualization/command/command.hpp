// Contains a general purpose command which features undo/redo.
// Soultatos Stefanos 2022

#ifndef COMMAND_COMMAND_HPP
#define COMMAND_COMMAND_HPP

#include <memory>

namespace visualization::command
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

} // namespace visualization::command

#endif // COMMAND_COMMAND_HPP
