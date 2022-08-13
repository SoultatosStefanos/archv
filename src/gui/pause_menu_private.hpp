// Contains a private module which provides the gui for the pause menu.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_PRIVATE_HPP
#define GUI_PAUSE_MENU_PRIVATE_HPP

#include <boost/signals2/signal.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// TODO Move these someplace else

namespace gui::detail
{

/***********************************************************
 * Pause window                                            *
 ***********************************************************/

// TODO Pass config data?
class pause_window
{
    using dependency_signal
        = boost::signals2::signal< void(const std::string&, double) >;

    using layout_signal = boost::signals2::signal< void(const std::string&) >;
    using topology_signal = boost::signals2::signal< void(const std::string&) >;
    using scale_signal = boost::signals2::signal< void(double) >;

public:
    using dependencies_table = std::unordered_map< std::string, int >;
    using layout_options = std::vector< std::string >;
    using topology_options = std::vector< std::string >;
    using scale_options = std::vector< double >;

    using dependency_slot = dependency_signal::slot_type;
    using layout_slot = layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using connection = boost::signals2::connection;

    pause_window(
        dependencies_table deps = dependencies_table(),
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    void draw() const;

    auto connect_to_dependency(const dependency_slot& slot) -> connection
    {
        return m_dependency_signal.connect(slot);
    }

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
    dependencies_table m_dependencies;
    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;

    dependency_signal m_dependency_signal;
    layout_signal m_layout_signal;
    topology_signal m_topology_signal;
    scale_signal m_scale_signal;
};

/***********************************************************
 * Menu Bar                                                *
 ***********************************************************/

class menu_bar
{
public:
    void draw() const;
};

} // namespace gui::detail

#endif // GUI_PAUSE_MENU_PRIVATE_HPP
