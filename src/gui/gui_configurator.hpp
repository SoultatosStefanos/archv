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

public:
    using string_slot_type = string_signal::slot_type;
    using int_slot_type = int_signal::slot_type;
    using bool_slot_type = bool_signal::slot_type;
    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto connect_to_color_theme(const string_slot_type& f) -> connection_type
    {
        return m_color_theme_sig.connect(f);
    }

    auto connect_to_font_name(const string_slot_type& f) -> connection_type
    {
        return m_font_name_sig.connect(f);
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

private:
    auto render_color_theme_selector() const -> void;
    auto render_font_selector() const -> void;
    auto render_frame_rounding_selector() const -> void;
    auto render_window_bordered_selector() const -> void;
    auto render_frame_bordered_selector() const -> void;
    auto render_popup_bordered_selector() const -> void;

    string_signal m_color_theme_sig, m_font_name_sig;
    int_signal m_frame_rounding_sig;
    bool_signal m_window_bordered_sig, m_frame_bordered_sig,
        m_popup_bordered_sig;
};

} // namespace gui

#endif // GUI_GUI_CONFIGURATOR_HPP
