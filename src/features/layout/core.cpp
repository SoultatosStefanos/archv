#include "core.hpp"

namespace features::layout
{

void core::initialize(command_history& cmds,
                      const graph& g,
                      vertex_id_map vertex_id,
                      weight_map edge_weight,
                      const Ogre::SceneManager& scene,
                      layout_descriptor layout_desc,
                      topology_descriptor topology_desc,
                      topology_scale scale)
{
    m_topology = topology_factory::make_topology(topology_desc, scale);
    m_layout =
        layout_factory::make_layout(layout_desc, g, *m_topology, edge_weight);

    m_update_layout = std::make_unique<update_layout_service>(
        cmds, g, edge_weight, m_layout, m_topology);
    m_update_topology = std::make_unique<update_topology_service>(
        cmds, g, edge_weight, m_layout, m_topology);

    m_view = std::make_unique<view>(scene);

    m_presenter = std::make_unique<core_presenter>(
        g, vertex_id, *m_view, *m_update_layout, *m_update_topology);

    m_presenter->update_view(*m_layout, *m_topology);
}

} // namespace features::layout