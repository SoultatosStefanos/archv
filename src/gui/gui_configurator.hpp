// Contains the GUI configurator interface.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_CONFIGURATOR_HPP
#define GUI_GUI_CONFIGURATOR_HPP

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string_view>
#include <vector>

namespace gui
{

class gui_configurator
{
public:
    using name_type = std::string_view;
    using rounding_type = int;
    using bordered_type = bool;

    using name_accessor = std::function< name_type() >;
    using rounding_accessor = std::function< rounding_type() >;
    using bordered_accessor = std::function< bordered_type() >;

private:
    using name_signal = boost::signals2::signal< void(name_type) >;
    using rounding_signal = boost::signals2::signal< void(rounding_type) >;
    using bordered_signal = boost::signals2::signal< void(bordered_type) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using name_slot = name_signal::slot_type;
    using rounding_slot = rounding_signal::slot_type;
    using bordered_slot = bordered_signal::slot_type;

    using apply_slot = apply_signal::slot_type;
    using preview_slot = preview_signal::slot_type;
    using cancel_slot = cancel_signal::slot_type;
    using restore_slot = restore_signal::slot_type;

    using connection = boost::signals2::connection;

    gui_configurator();

    auto render() const -> void;

    auto color_theme() const -> name_type;
    auto frame_rounding() const -> rounding_type;
    auto window_bordered() const -> bordered_type;
    auto frame_bordered() const -> bordered_type;
    auto popup_bordered() const -> bordered_type;

    auto set_color_theme(name_accessor f) -> void;
    auto set_frame_rounding(rounding_accessor f) -> void;
    auto set_window_bordered(bordered_accessor f) -> void;
    auto set_frame_bordered(bordered_accessor f) -> void;
    auto set_popup_bordered(bordered_accessor f) -> void;

    auto connect_to_color_theme(const name_slot& f) -> connection;
    auto connect_to_frame_rounding(const rounding_slot& f) -> connection;
    auto connect_to_window_bordered(const bordered_slot& f) -> connection;
    auto connect_to_frame_bordered(const bordered_slot& f) -> connection;
    auto connect_to_popup_bordered(const bordered_slot& f) -> connection;
    auto connect_to_apply(const apply_slot& f) -> connection;
    auto connect_to_preview(const preview_slot& f) -> connection;
    auto connect_to_cancel(const cancel_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    auto emit_color_theme(name_type theme) const -> void;
    auto emit_frame_rounding(rounding_type r) const -> void;
    auto emit_window_bordered(bordered_type b) const -> void;
    auto emit_frame_bordered(bordered_type b) const -> void;
    auto emit_popup_bordered(bordered_type b) const -> void;
    auto emit_apply() const -> void;
    auto emit_preview() const -> void;
    auto emit_cancel() const -> void;
    auto emit_restore() const -> void;

protected:
    using render_vector = std::vector< const char* >;

    auto color_themes() const -> const render_vector& { return m_color_themes; }
    auto color_themes() -> render_vector& { return m_color_themes; }

private:
    using index_type = int;

    auto render_color_theme_selector() const -> void;
    auto render_frame_rounding_selector() const -> void;
    auto render_window_bordered_selector() const -> void;
    auto render_frame_bordered_selector() const -> void;
    auto render_popup_bordered_selector() const -> void;
    auto render_config_buttons() const -> void;

    name_signal m_color_theme_sig;
    rounding_signal m_frame_rounding_sig;
    bordered_signal m_window_bordered_sig;
    bordered_signal m_frame_bordered_sig;
    bordered_signal m_popup_bordered_sig;
    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;

    name_accessor m_color_theme;
    rounding_accessor m_frame_rounding;
    bordered_accessor m_window_bordered;
    bordered_accessor m_frame_bordered;
    bordered_accessor m_popup_bordered;

    mutable render_vector m_color_themes;
};

} // namespace gui

#endif // GUI_GUI_CONFIGURATOR_HPP
