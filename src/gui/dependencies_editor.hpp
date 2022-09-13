// Contains a class responsible for the edge weights editiing gui.
// Soultatos Stefanos 2022

#ifndef GUI_DEPENDENCIES_EDITOR_HPP
#define GUI_DEPENDENCIES_EDITOR_HPP

#include "plugins.hpp"

#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>

namespace gui
{

class dependencies_editor
{
public:
    using dependency_type = plugins::dependency;
    using weight_type = plugins::weight;

private:
    using dependency_signal
        = boost::signals2::signal< void(dependency_type, weight_type) >;

    using restore_signal = boost::signals2::signal< void(void) >;

public:
    using dependency_slot_type = dependency_signal::slot_type;
    using restore_slot_type = restore_signal::slot_type;
    using connection_type = boost::signals2::connection;

    dependencies_editor();

    auto render() const -> void;

    auto connect_to_dependency(const dependency_slot_type& f) -> connection_type
    {
        return m_dependency_sig.connect(f);
    }

    auto connect_to_restore(const restore_slot_type& f) -> connection_type
    {
        return m_restore_sig.connect(f);
    }

private:
    dependency_signal m_dependency_sig;
    restore_signal m_restore_sig;

    mutable std::vector< std::string > m_weight_strs;
};

} // namespace gui

#endif // GUI_DEPENDENCIES_EDITOR_HPP
