// Contains a private module which provides the gui for the pause menu.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_PRIVATE_HPP
#define GUI_PAUSE_MENU_PRIVATE_HPP

#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>

namespace gui::detail
{

// TODO Pass config data?
class pause_menu_gui
{
    using layout_signal = boost::signals2::signal< void(const std::string&) >;
    using topology_signal = boost::signals2::signal< void(const std::string&) >;
    using scale_signal = boost::signals2::signal< void(double) >;

public:
    using layout_options = std::vector< std::string >;
    using topology_options = std::vector< std::string >;
    using scale_options = std::vector< double >;

    using layout_slot = layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using connection = boost::signals2::connection;

    pause_menu_gui(
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    void draw() const;

    auto connect_to_layout(const layout_slot& slot) -> connection
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot& slot) -> connection
    {
        return m_topology_signal.connect(slot);
    }

    auto connect_to_scale(const scale_slot& slot) -> connection
    {
        return m_scale_signal.connect(slot);
    }

private:
    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;

    layout_signal m_layout_signal;
    topology_signal m_topology_signal;
    scale_signal m_scale_signal;
};

} // namespace gui::detail

#endif // GUI_PAUSE_MENU_PRIVATE_HPP
