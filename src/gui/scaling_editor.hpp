// Contains the editor ui of the scaling subsystem.
// Soultatos Stefanos 2022

#ifndef GUI_SCALING_EDITOR_HPP
#define GUI_SCALING_EDITOR_HPP

#include <array>                     // for array
#include <boost/signals2/signal.hpp> // for signal, connection
#include <functional>                // for function
#include <string_view>               // for string_view
#include <vector>                    // for vector

namespace gui
{

class scaling_editor
{
public:
    using tag_type = std::string_view;
    using dims_type = std::array< bool, 3 >;
    using baseline_type = float;
    using enabled_type = bool;
    using ratio_type = float;

private:
    using dims_signal
        = boost::signals2::signal< void(tag_type, const dims_type&) >;
    using baseline_signal
        = boost::signals2::signal< void(tag_type, baseline_type) >;
    using enabled_signal
        = boost::signals2::signal< void(tag_type, enabled_type) >;

    using ratio_signal = boost::signals2::signal< void(tag_type, ratio_type) >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using dims_accessor = std::function< dims_type(tag_type) >;
    using baseline_accessor = std::function< baseline_type(tag_type) >;
    using enabled_accessor = std::function< enabled_type(tag_type) >;
    using ratio_accessor = std::function< ratio_type(tag_type) >;

    using dims_slot = dims_signal::slot_type;
    using baseline_slot = baseline_signal::slot_type;
    using enabled_slot = enabled_signal::slot_type;
    using ratio_slot = ratio_signal::slot_type;
    using restore_slot = restore_signal::slot_type;

    using connection = boost::signals2::connection;

    scaling_editor();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto dims(tag_type tag) const -> dims_type;
    auto baseline(tag_type tag) const -> baseline_type;
    auto enabled(tag_type tag) const -> enabled_type;
    auto min_ratio(tag_type tag) const -> ratio_type;
    auto max_ratio(tag_type tag) const -> ratio_type;

    auto set_dims(dims_accessor f) -> void;
    auto set_baseline(baseline_accessor f) -> void;
    auto set_enabled(enabled_accessor f) -> void;
    auto set_min_ratio(ratio_accessor f) -> void;
    auto set_max_ratio(ratio_accessor f) -> void;

    auto connect_to_dims(const dims_slot& f) -> connection;
    auto connect_to_baseline(const baseline_slot& f) -> connection;
    auto connect_to_enabled(const enabled_slot& f) -> connection;
    auto connect_to_min_ratio(const ratio_slot& f) -> connection;
    auto connect_to_max_ratio(const ratio_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto factors() const -> const render_vector& { return m_factors; }
    auto factors() -> render_vector& { return m_factors; }

    auto emit_factor_dims(tag_type tag, const dims_type& dims) const -> void;
    auto emit_factor_baseline(tag_type tag, baseline_type b) const -> void;
    auto emit_factor_enabled(tag_type tag, enabled_type enabled) const -> void;
    auto emit_factor_min_ratio(tag_type tag, ratio_type ratio) const -> void;
    auto emit_factor_max_ratio(tag_type tag, ratio_type ratio) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_factors() const -> void;
    auto render_factor(tag_type tag) const -> void;
    auto render_dims_editor(tag_type tag) const -> void;
    auto render_baseline_editor(tag_type tag) const -> void;
    auto render_enabled_editor(tag_type tag) const -> void;
    auto render_ratios_editor(tag_type tag) const -> void;
    auto render_restore_button() const -> void;

    mutable bool m_visible { false };
    dims_signal m_dims_sig;
    baseline_signal m_baseline_sig;
    enabled_signal m_enabled_sig;
    ratio_signal m_min_ratio_sig;
    ratio_signal m_max_ratio_sig;
    restore_signal m_restore_sig;

    dims_accessor m_dims;
    baseline_accessor m_baseline;
    enabled_accessor m_enabled;
    ratio_accessor m_min_ratio;
    ratio_accessor m_max_ratio;

    mutable render_vector m_factors;
};

} // namespace gui

#endif // GUI_SCALING_EDITOR_HPP
