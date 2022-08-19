// Contains the pause menu overlay of the application.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_HPP
#define GUI_PAUSE_MENU_HPP

#include "overlay.hpp"

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace gui
{

class pause_menu : public overlay
{
    using dependency_signal
        = boost::signals2::signal< void(const std::string&, int) >;

    using layout_signal = boost::signals2::signal< void(const std::string&) >;
    using topology_signal = boost::signals2::signal< void(const std::string&) >;
    using scale_signal = boost::signals2::signal< void(double) >;
    using dependencies_restore_signal = boost::signals2::signal< void() >;
    using layout_restore_signal = boost::signals2::signal< void() >;

    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using dependency_options = std::unordered_map< std::string, int >;
    using layout_options = std::unordered_set< std::string >;
    using topology_options = std::unordered_set< std::string >;
    using scale_options = std::unordered_set< double >;

    using dependency_slot = dependency_signal::slot_type;
    using layout_slot = layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using dependencies_restore_slot = dependencies_restore_signal::slot_type;
    using layout_restore_slot = layout_restore_signal::slot_type;

    using undo_enabled = std::function< bool() >;
    using redo_enabled = std::function< bool() >;

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;

    using connection = boost::signals2::connection;

    explicit pause_menu(
        dependency_options deps = dependency_options(),
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options(),
        undo_enabled is_undo_enabled = []() { return false; },
        redo_enabled is_redo_enabled = []() { return false; });

    virtual ~pause_menu() = default;

    virtual auto draw() const -> void override;

    auto connect_to_dependency(const dependency_slot& slot) -> connection
    {
        return m_dependency_signal.connect(slot);
    }

    auto connect_to_layout(const layout_slot& slot) -> connection
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot& slot) -> connection
    {
        return m_topology_signal.connect(slot);
    }

    auto connect_to_scale(const scale_slot& slot) -> connection
    {
        return m_scale_signal.connect(slot);
    }

    auto connect_to_dependencies_restore(const dependencies_restore_slot& slot)
        -> connection
    {
        return m_dependencies_restore_sig.connect(slot);
    }

    auto connect_to_layout_restore(const layout_restore_slot& slot)
        -> connection
    {
        return m_layout_restore_sig.connect(slot);
    }

    auto connect_to_undo(const undo_slot& f) -> connection
    {
        return m_undo_sig.connect(f);
    }

    auto connect_to_redo(const redo_slot& f) -> connection
    {
        return m_redo_sig.connect(f);
    }

    auto set_dependency(const std::string& type, int weight) -> void;
    auto set_layout(const std::string& type) -> void;
    auto set_topology(const std::string& type) -> void;
    auto set_scale(double val) -> void;

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
    auto draw_dependencies_header() const -> void;
    auto draw_layout_header() const -> void;
    auto draw_clustering_header() const -> void;
    auto draw_code_inspection_header() const -> void;
    auto draw_file_submenu() const -> void;
    auto draw_edit_submenu() const -> void;

    dependency_options m_dependencies;
    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;

    dependency_signal m_dependency_signal;
    layout_signal m_layout_signal;
    topology_signal m_topology_signal;
    scale_signal m_scale_signal;
    dependencies_restore_signal m_dependencies_restore_sig;
    layout_restore_signal m_layout_restore_sig;

    undo_enabled m_undo_enabled;
    redo_enabled m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;

    mutable std::vector< std::string > m_weight_strs;
    mutable int m_selected_layout { -1 };
    mutable int m_selected_topology { -1 };
    mutable int m_selected_scale { -1 };
};

} // namespace gui

#endif // GUI_PAUSE_MENU_HPP
