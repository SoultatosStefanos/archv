// Contains a class responsible for the graph layout editing gui.
// Soultatos Stefanos 2022

#ifndef GUI_LAYOUT_EDITOR_HPP
#define GUI_LAYOUT_EDITOR_HPP

#include <boost/signals2/signal.hpp>

namespace gui
{

class layout_editor
{
public:
    using layout_type = const char*;
    using topology_type = const char*;
    using scale_type = float;

private:
    using layout_signal = boost::signals2::signal< void(layout_type) >;
    using topology_signal = boost::signals2::signal< void(topology_type) >;
    using scale_signal = boost::signals2::signal< void(scale_type) >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using layout_slot = layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using restore_slot = restore_signal::slot_type;
    using connection = boost::signals2::connection;

    auto render() const -> void;

    auto layout() const -> layout_type;
    auto topology() const -> topology_type;
    auto scale() const -> scale_type;

    auto set_layout(layout_type val) -> void;
    auto set_topology(topology_type val) -> void;
    auto set_scale(scale_type val) -> void;

    auto connect_to_layout(const layout_slot& f) -> connection;
    auto connect_to_topology(const topology_slot& f) -> connection;
    auto connect_to_scale(const scale_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    auto emit_layout() const -> void;
    auto emit_topology() const -> void;
    auto emit_scale() const -> void;
    auto emit_restore() const -> void;

private:
    using index_type = int;

    auto render_layout_editor() const -> void;
    auto render_topology_editor() const -> void;
    auto render_scale_editor() const -> void;
    auto render_restore_button() const -> void;

    layout_signal m_layout_sig;
    topology_signal m_topology_sig;
    scale_signal m_scale_sig;
    restore_signal m_restore_sig;

    mutable index_type m_layout { 0 };
    mutable index_type m_topology { 0 };
    mutable scale_type m_scale { 0 };
};

} // namespace gui

#endif // GUI_LAYOUT_EDITOR_HPP
