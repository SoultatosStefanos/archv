// Contains an interface for undoable/redoable general purpose actions.
// Soultatos Stefanos 2022.

#ifndef UNDO_REDO_COMMAND_HPP
#define UNDO_REDO_COMMAND_HPP

namespace undo_redo
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

    virtual auto execute() -> void = 0;
    virtual auto undo() -> void = 0;
    virtual auto redo() -> void = 0;
};

} // namespace undo_redo

#endif // UNDO_REDO_COMMAND_HPP
