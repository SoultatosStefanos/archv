// Contains the pause menu gui of the application.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_HPP
#define GUI_PAUSE_MENU_HPP

#include <MYGUI/MyGUI.h>
#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>

namespace gui
{

// Pause menu gui.
class pause_menu
{
    using layout_signal = boost::signals2::signal< void(const std::string&) >;
    using topology_signal = boost::signals2::signal< void(const std::string&) >;
    using scale_signal = boost::signals2::signal< void(double) >;

public:
    using layout_slot = layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using connection = boost::signals2::connection;

    using layout_options = std::vector< std::string >;
    using topology_options = std::vector< std::string >;
    using scale_options = std::vector< double >;

    pause_menu(
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    ~pause_menu();

    pause_menu(const pause_menu&) = delete;
    pause_menu(pause_menu&&) = delete;

    auto operator=(const pause_menu&) -> pause_menu& = delete;
    auto operator=(pause_menu&&) -> pause_menu& = delete;

    void show();
    void hide();

    auto connect_to_layout(const layout_slot& slot) -> connection
    {
        return m_layout_sig.connect(slot);
    }

    auto connect_to_topology(const topology_slot& slot) -> connection
    {
        return m_topology_sig.connect(slot);
    }

    auto connect_to_scale(const scale_slot& slot) -> connection
    {
        return m_scale_sig.connect(slot);
    }

protected:
    void emit_layout(MyGUI::Widget* from) const;
    void emit_topology(MyGUI::Widget* from) const;
    void emit_scale(MyGUI::Widget* from) const;

private:
    layout_signal m_layout_sig;
    topology_signal m_topology_sig;
    scale_signal m_scale_sig;

    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;

    MyGUI::MenuBar* m_root { nullptr };
    MyGUI::VectorWidgetPtr m_widgets;
};

} // namespace gui

#endif // GUI_PAUSE_MENU_HPP
