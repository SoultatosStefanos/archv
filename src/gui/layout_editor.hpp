// Contains a class responsible for the graph layout editing gui.
// Soultatos Stefanos 2022

#ifndef GUI_LAYOUT_EDITOR_HPP
#define GUI_LAYOUT_EDITOR_HPP

#include <boost/signals2/signal.hpp> // for signal
#include <functional>                // for function
#include <string_view>               // for string_view
#include <vector>                    // for vector

namespace gui
{

class layout_editor
{
public:
    using layout_type = std::string_view;
    using topology_type = std::string_view;
    using scale_type = float;

    using layout_accessor = std::function< layout_type() >;
    using topology_accessor = std::function< topology_type() >;
    using scale_accessor = std::function< scale_type() >;

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

    layout_editor();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto layout() const -> layout_type;
    auto topology() const -> topology_type;
    auto scale() const -> scale_type;

    auto set_layout(layout_accessor f) -> void;
    auto set_topology(topology_accessor f) -> void;
    auto set_scale(scale_accessor f) -> void;

    auto connect_to_layout(const layout_slot& f) -> connection;
    auto connect_to_topology(const topology_slot& f) -> connection;
    auto connect_to_scale(const scale_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto layouts() const -> const render_vector& { return m_layouts; }
    auto layouts() -> render_vector& { return m_layouts; }

    auto topologies() const -> const render_vector& { return m_topologies; }
    auto topologies() -> render_vector& { return m_topologies; }

    auto emit_layout(layout_type l) const -> void;
    auto emit_topology(topology_type t) const -> void;
    auto emit_scale(scale_type s) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_layout_editor() const -> void;
    auto render_topology_editor() const -> void;
    auto render_scale_editor() const -> void;
    auto render_restore_button() const -> void;

    mutable bool m_visible { false };
    layout_signal m_layout_sig;
    topology_signal m_topology_sig;
    scale_signal m_scale_sig;
    restore_signal m_restore_sig;

    layout_accessor m_layout;
    topology_accessor m_topology;
    scale_accessor m_scale;

    // For rendering only.
    mutable render_vector m_layouts;
    mutable render_vector m_topologies;
};

} // namespace gui

#endif // GUI_LAYOUT_EDITOR_HPP
