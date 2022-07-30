#include "core.hpp"

namespace features::layout
{

core::core(command_history& cmds,
           const graph& g,
           weight_map edge_weight,
           const std::string& layout_type,
           const std::string& topolgy_type,
           double topology_scale)
    : m_topology{topology_factory::make_topology(topolgy_type, topology_scale)},
      m_layout{layout_factory::make_layout(
          layout_type, g, *m_topology, edge_weight)},
      m_update_layout{cmds, g, edge_weight, m_layout, m_topology},
      m_update_topology{cmds, g, edge_weight, m_layout, m_topology},
      m_revert_to_defaults{cmds,
                           g,
                           edge_weight,
                           layout_type,
                           topolgy_type,
                           topology_scale,
                           m_layout,
                           m_topology}
{
    m_update_layout.connect(
        [this](const auto& l, const auto& s) { m_signal(l, s); });
    m_update_topology.connect(
        [this](const auto& l, const auto& s) { m_signal(l, s); });
    m_revert_to_defaults.connect(
        [this](const auto& l, const auto& s) { m_signal(l, s); });
}

auto core::get_layout() const -> const layout&
{
    assert(m_layout);
    return *m_layout;
}

auto core::get_topology() const -> const topology&
{
    assert(m_topology);
    return *m_topology;
}

void core::update_layout(const std::string& type)
{
    m_update_layout.execute(type);
}

void core::update_topology(const std::string& type, double scale)
{
    m_update_topology.execute(type, scale);
}

void core::revert_to_defaults()
{
    m_revert_to_defaults.execute();
}

auto core::connect(const slot_type& slot) -> connection
{
    return m_signal.connect(slot);
}

} // namespace features::layout