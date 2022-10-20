// Contains the editor gui menu.
// Soultatos Stefanos 2022

#ifndef GUI_EDITOR_HPP
#define GUI_EDITOR_HPP

#include "dependencies_editor.hpp"
#include "layout_editor.hpp"
#include "scaling_editor.hpp"

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

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;
    using connection = boost::signals2::connection;

    auto render() const -> void;

    auto get_dependencies_editor() const -> const auto& { return m_deps_edit; }
    auto get_dependencies_editor() -> auto& { return m_deps_edit; }

    auto get_layout_editor() const -> const auto& { return m_layout_edit; }
    auto get_layout_editor() -> auto& { return m_layout_edit; }

    auto get_scaling_editor() const -> const auto& { return m_scaling_edit; }
    auto get_scaling_editor() -> auto& { return m_scaling_edit; }

    auto set_can_undo(undo_enabled_pred pred) -> void
    {
        assert(pred);
        m_undo_enabled = std::move(pred);
    }

    auto set_can_redo(redo_enabled_pred pred) -> void
    {
        assert(pred);
        m_redo_enabled = std::move(pred);
    }

    auto undo_shortcut() -> void;
    auto redo_shortcut() -> void;

    auto connect_to_undo(const undo_slot& f) -> connection
    {
        return m_undo_sig.connect(f);
    }

    auto connect_to_redo(const redo_slot& f) -> connection
    {
        return m_redo_sig.connect(f);
    }

protected:
    auto emit_undo() const -> void;
    auto emit_redo() const -> void;

private:
    auto render_undo_redo() const -> void;
    auto render_dependencies_editor_item() const -> void;
    auto render_layout_editor_item() const -> void;
    auto render_scaling_editor_item() const -> void;
    auto render_dependencies_editor() const -> void;
    auto render_layout_editor() const -> void;
    auto render_scaling_editor() const -> void;

    undo_enabled_pred m_undo_enabled;
    redo_enabled_pred m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;

    dependencies_editor m_deps_edit;
    layout_editor m_layout_edit;
    scaling_editor m_scaling_edit;

    mutable bool m_dependencies_open { false };
    mutable bool m_layout_open { false };
    mutable bool m_scaling_open { false };
};

} // namespace gui

#endif // GUI_EDITOR_HPP
