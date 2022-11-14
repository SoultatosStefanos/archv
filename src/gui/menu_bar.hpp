// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include "overlay.hpp"

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string>

namespace gui
{

/***********************************************************
 * Menu Bar                                                *
 ***********************************************************/

class menu_bar : public overlay
{
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using pred = std::function< bool() >;

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;
    using connection = boost::signals2::connection;

    static constexpr auto type_id = "menu_bar";

    menu_bar() = default;
    ~menu_bar() override = default;

    auto id() const -> id_type override { return type_id; }

    auto weights_editor() const -> auto* { return m_weights_editor; }
    auto set_weights_editor(overlay* o) { m_weights_editor = o; }

    auto layout_editor() const -> auto* { return m_layout_editor; }
    auto set_layout_editor(overlay* o) { m_layout_editor = o; }

    auto scaling_editor() const -> auto* { return m_scaling_editor; }
    auto set_scaling_editor(overlay* o) { m_scaling_editor = o; }

    auto degrees_editor() const -> auto* { return m_degrees_editor; }
    auto set_degrees_editor(overlay* o) { m_degrees_editor = o; }

    auto clustering_editor() const -> auto* { return m_c_editor; }
    auto set_clustering_editor(overlay* o) { m_c_editor = o; }

    auto background_configurator() const -> auto* { return m_bkg_cfg; }
    auto set_background_configurator(overlay* o) { m_bkg_cfg = o; }

    auto graph_configurator() const -> auto* { return m_graph_cfg; }
    auto set_graph_configurator(overlay* o) { m_graph_cfg = o; }

    auto gui_configurator() const -> auto* { return m_gui_cfg; }
    auto set_gui_configurator(overlay* o) { m_gui_cfg = o; }

    auto visible() const -> bool override { return m_visible; }
    auto show() -> void override { m_visible = true; }
    auto hide() -> void override { m_visible = false; }

    auto render() const -> void override;

    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    auto set_can_undo(pred pred) -> void;
    auto set_can_redo(pred pred) -> void;

    auto connect_to_undo(const undo_slot& f) -> connection;
    auto connect_to_redo(const redo_slot& f) -> connection;

protected:
    auto emit_undo() const -> void;
    auto emit_redo() const -> void;

private:
    auto render_file_editor() const -> void;
    auto render_editor() const -> void;
    auto render_configurator() const -> void;
    auto render_rendering_configurator() const -> void;
    auto render_gui_configurator() const -> void;
    auto render_helper() const -> void;

    bool m_visible { true };
    overlay* m_weights_editor { nullptr };
    overlay* m_layout_editor { nullptr };
    overlay* m_scaling_editor { nullptr };
    overlay* m_degrees_editor { nullptr };
    overlay* m_c_editor { nullptr };
    overlay* m_bkg_cfg { nullptr };
    overlay* m_graph_cfg { nullptr };
    overlay* m_gui_cfg { nullptr };

    pred m_undo_enabled;
    pred m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;
};

} // namespace gui

#endif // GUI_MENU_BAR_HPP
