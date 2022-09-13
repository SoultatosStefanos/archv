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
    using scale_type = double;

private:
    using layout_signal = boost::signals2::signal< void(layout_type) >;
    using topology_signal = boost::signals2::signal< void(topology_type) >;
    using scale_signal = boost::signals2::signal< void(scale_type) >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using layout_slot_type = layout_signal::slot_type;
    using topology_slot_type = topology_signal::slot_type;
    using scale_slot_type = scale_signal::slot_type;
    using restore_slot_type = restore_signal::slot_type;
    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto set_layout(layout_type val) -> void;
    auto set_topology(topology_type val) -> void;
    auto set_scale(scale_type val) -> void;

    auto connect_to_layout(const layout_slot_type& f) -> connection_type
    {
        return m_layout_sig.connect(f);
    }

    auto connect_to_topology(const topology_slot_type& f) -> connection_type
    {
        return m_topology_sig.connect(f);
    }

    auto connect_to_scale(const scale_slot_type& f) -> connection_type
    {
        return m_scale_sig.connect(f);
    }

    auto connect_to_restore(const restore_slot_type& f) -> connection_type
    {
        return m_restore_sig.connect(f);
    }

private:
    auto render_layout_editor() const -> void;
    auto render_topology_editor() const -> void;
    auto render_scale_editor() const -> void;
    auto render_restore_button() const -> void;

    layout_signal m_layout_sig;
    topology_signal m_topology_sig;
    scale_signal m_scale_sig;
    restore_signal m_restore_sig;

    mutable int m_selected_layout { -1 };
    mutable int m_selected_topology { -1 };
    mutable float m_selected_scale { -1 };
};

} // namespace gui

#endif // GUI_LAYOUT_EDITOR_HPP
