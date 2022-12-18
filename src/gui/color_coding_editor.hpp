// Contains the color coding gui editor.
// Soultatos Stefanos 2022

#ifndef GUI_COLOR_CODING_EDITOR_HPP
#define GUI_COLOR_CODING_EDITOR_HPP

#include <array>                     // for array
#include <boost/signals2/signal.hpp> // for signal, connection
#include <functional>                // for function
#include <string_view>               // for string_view

namespace gui
{

/***********************************************************
 * Color coding editor                                      *
 ***********************************************************/

class color_coding_editor
{
public:
    using dependency_type = std::string_view;
    using rgba_type = std::array< float, 4 >;

    using rgba_accessor = std::function< rgba_type(dependency_type) >;
    using active_accessor = std::function< bool(dependency_type) >;

private:
    using rgba_signal
        = boost::signals2::signal< void(dependency_type, rgba_type) >;
    using active_signal
        = boost::signals2::signal< void(dependency_type, bool) >;

    using restore_signal = boost::signals2::signal< void() >;

public:
    using rgba_slot = rgba_signal::slot_type;
    using active_slot = active_signal::slot_type;
    using restore_slot = restore_signal::slot_type;
    using connection = boost::signals2::connection;

    color_coding_editor();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto color(dependency_type dependency) const -> rgba_type;
    auto active(dependency_type dependency) const -> bool;

    auto set_color(rgba_accessor f) -> void;
    auto set_active(active_accessor f) -> void;

    auto connect_to_color(const rgba_slot& f) -> connection;
    auto connect_to_active(const active_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto dependencies() const -> const render_vector& { return m_dependencies; }
    auto dependencies() -> render_vector& { return m_dependencies; }

    auto emit(dependency_type val, rgba_type rgba) const -> void;
    auto emit(dependency_type val, bool active) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_dependencies() const -> void;
    auto render_restore_button() const -> void;

    mutable bool m_visible { false };
    rgba_signal m_rgba_sig;
    active_signal m_active_sig;
    restore_signal m_restore_sig;
    rgba_accessor m_rgba;
    active_accessor m_active;

    mutable render_vector m_dependencies;
};

} // namespace gui

#endif // GUI_COLOR_CODING_EDITOR_HPP
