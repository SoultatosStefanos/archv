#include "core.hpp"

namespace features::layout
{

void core::initialize(command_history& cmds,
                      const graph& g,
                      weight_map edge_weight,
                      const std::string& layout_type,
                      const std::string& topolgy_type,
                      double topology_scale)
{
    m_topology = topology_factory::make_topology(topolgy_type, topology_scale);
    m_layout =
        layout_factory::make_layout(layout_type, g, *m_topology, edge_weight);

    m_update_layout = std::make_unique<update_layout_service>(
        cmds, g, edge_weight, m_layout, m_topology);

    m_update_topology = std::make_unique<update_topology_service>(
        cmds, g, edge_weight, m_layout, m_topology);

    m_revert_to_defaults =
        std::make_unique<revert_to_defaults_service>(cmds,
                                                     g,
                                                     edge_weight,
                                                     layout_type,
                                                     topolgy_type,
                                                     topology_scale,
                                                     m_layout,
                                                     m_topology);
}

void core::reset()
{
    m_topology.reset();
    m_layout.reset();
    m_update_layout.reset();
    m_update_topology.reset();
    m_revert_to_defaults.reset();
}

auto core::get_layout_type() const -> std::string
{
    assert(m_layout);
    return m_layout->desc();
}

auto core::get_topology_type() const -> std::string
{
    assert(m_topology);
    return m_topology->desc();
}

auto core::get_topology_scale() const -> double
{
    assert(m_topology);
    return m_topology->scale();
}

void core::update_layout(const std::string& type)
{
    assert(m_update_layout);
    m_update_layout->execute(type);
}

void core::update_topology(const std::string& type, double scale)
{
    assert(m_update_topology);
    m_update_topology->execute(type, scale);
}

void core::revert_to_defaults()
{
    assert(m_revert_to_defaults);
    m_revert_to_defaults->execute();
}

void core::connect(const slot_type& slot)
{
    assert(m_update_layout);
    assert(m_update_topology);
    assert(m_revert_to_defaults);

    m_update_layout->connect(slot);
    m_update_topology->connect(slot);
    m_revert_to_defaults->connect(slot);
}

} // namespace features::layout