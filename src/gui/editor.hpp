// Contains the editor gui menu.
// Soultatos Stefanos 2022

#ifndef GUI_EDITOR_HPP
#define GUI_EDITOR_HPP

#include "dependencies_editor.hpp"
#include "layout_editor.hpp"

#include <boost/signals2/signal.hpp>
#include <cassert>
#include <functional>

namespace gui
{

class editor
{
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using undo_enabled_pred = std::function< bool() >;
    using redo_enabled_pred = std::function< bool() >;

    using undo_slot_type = undo_signal::slot_type;
    using redo_slot_type = redo_signal::slot_type;
    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto get_dependencies_editor() const -> const auto& { return m_deps_edit; }
    auto get_dependencies_editor() -> auto& { return m_deps_edit; }

    auto get_layout_editor() const -> const auto& { return m_layout_edit; }
    auto get_layout_editor() -> auto& { return m_layout_edit; }

    auto set_undo_enabled(undo_enabled_pred pred) -> void
    {
        assert(pred);
        m_undo_enabled = std::move(pred);
    }

    auto set_redo_enabled(redo_enabled_pred pred) -> void
    {
        assert(pred);
        m_redo_enabled = std::move(pred);
    }

    auto connect_to_undo(const undo_slot_type& f) -> connection_type
    {
        return m_undo_sig.connect(f);
    }

    auto connect_to_redo(const redo_slot_type& f) -> connection_type
    {
        return m_redo_sig.connect(f);
    }

private:
    auto render_undo_redo() const -> void;
    auto render_dependencies_editor() const -> void;
    auto render_layout_editor() const -> void;

    undo_enabled_pred m_undo_enabled;
    redo_enabled_pred m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;

    dependencies_editor m_deps_edit;
    layout_editor m_layout_edit;
};

} // namespace gui

#endif // GUI_EDITOR_HPP
