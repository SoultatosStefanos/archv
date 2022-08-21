// Contains a class which encapsulates the entire menu bar gui overlay.
// Soultatos Stefanos 2022

#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include "overlay.hpp"

#include <boost/signals2/signal.hpp>
#include <cassert>
#include <functional>

namespace gui
{

class menu_bar : public overlay
{
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using undo_enabled = std::function< bool() >;
    using redo_enabled = std::function< bool() >;

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;

    using connection = boost::signals2::connection;

    explicit menu_bar(
        undo_enabled is_undo_enabled = []() { return false; },
        redo_enabled is_redo_enabled = []() { return false; });

    virtual ~menu_bar() = default;

    virtual auto draw() const -> void override;

    auto connect_to_undo(const undo_slot& f) -> connection
    {
        return m_undo_sig.connect(f);
    }

    auto connect_to_redo(const redo_slot& f) -> connection
    {
        return m_redo_sig.connect(f);
    }

    auto set_undo_enabled(undo_enabled pred) -> void
    {
        assert(pred);
        m_undo_enabled = std::move(pred);
    }

    auto set_redo_enabled(redo_enabled pred) -> void
    {
        assert(pred);
        m_redo_enabled = std::move(pred);
    }

    auto undo_shortcut() -> void;
    auto redo_shortcut() -> void;

private:
    auto draw_file_submenu() const -> void;
    auto draw_edit_submenu() const -> void;

    undo_enabled m_undo_enabled;
    redo_enabled m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;
};

} // namespace gui

#endif // GUI_MENU_BAR_HPP
