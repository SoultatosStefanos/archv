// Contains the GUI configurator interface.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_CONFIGURATOR_HPP
#define GUI_GUI_CONFIGURATOR_HPP

#include <boost/signals2/signal.hpp>

namespace gui
{

class gui_configurator
{
    using string_signal = boost::signals2::signal< void(const char*) >;
    using int_signal = boost::signals2::signal< void(int) >;
    using bool_signal = boost::signals2::signal< void(bool) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using string_slot_type = string_signal::slot_type;
    using int_slot_type = int_signal::slot_type;
    using bool_slot_type = bool_signal::slot_type;

    using apply_slot_type = apply_signal::slot_type;
    using preview_slot_type = preview_signal::slot_type;
    using cancel_slot_type = cancel_signal::slot_type;
    using restore_slot_type = restore_signal::slot_type;

    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto set_color_theme(const char* theme) -> void;
    auto set_frame_rounding(int value) -> void;
    auto set_window_bordered(bool toggle) -> void;
    auto set_frame_bordered(bool toggle) -> void;
    auto set_popup_bordered(bool toggle) -> void;

    auto connect_to_color_theme(const string_slot_type& f) -> connection_type
    {
        return m_color_theme_sig.connect(f);
    }

    auto connect_to_frame_rounding(const int_slot_type& f) -> connection_type
    {
        return m_frame_rounding_sig.connect(f);
    }

    auto connect_to_window_bordered(const bool_slot_type& f) -> connection_type
    {
        return m_window_bordered_sig.connect(f);
    }

    auto connect_to_frame_bordered(const bool_slot_type& f) -> connection_type
    {
        return m_frame_bordered_sig.connect(f);
    }

    auto connect_to_popup_bordered(const bool_slot_type& f) -> connection_type
    {
        return m_popup_bordered_sig.connect(f);
    }

    auto connect_to_apply(const apply_slot_type& f) -> connection_type
    {
        return m_apply_sig.connect(f);
    }

    auto connect_to_preview(const preview_slot_type& f) -> connection_type
    {
        return m_preview_sig.connect(f);
    }

    auto connect_to_cancel(const cancel_slot_type& f) -> connection_type
    {
        return m_cancel_sig.connect(f);
    }

    auto connect_to_restore(const restore_slot_type& f) -> connection_type
    {
        return m_restore_sig.connect(f);
    }

private:
    auto render_color_theme_selector() const -> void;
    auto render_frame_rounding_selector() const -> void;
    auto render_window_bordered_selector() const -> void;
    auto render_frame_bordered_selector() const -> void;
    auto render_popup_bordered_selector() const -> void;
    auto render_config_buttons() const -> void;

    string_signal m_color_theme_sig;
    int_signal m_frame_rounding_sig;
    bool_signal m_window_bordered_sig, m_frame_bordered_sig,
        m_popup_bordered_sig;

    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;

    mutable int m_color_theme_selected { 0 };
    mutable int m_frame_rounding_selected { 0 };
    mutable bool m_window_bordered_selected { false };
    mutable bool m_frame_bordered_selected { false };
    mutable bool m_popup_bordered_selected { false };
};

} // namespace gui

#endif // GUI_GUI_CONFIGURATOR_HPP
