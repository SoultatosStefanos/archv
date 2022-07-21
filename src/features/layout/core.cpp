#include "core.hpp"

namespace features::layout
{

void core::initialize(command_history& cmds,
                      const graph& g,
                      const Ogre::SceneManager& scene,
                      layout_descriptor layout_desc,
                      topology_descriptor topology_desc,
                      topology_scale scale)
{
    m_topology = topology_factory::make_topology(topology_desc, scale);
    m_layout = layout_factory::make_layout(layout_desc, g, *m_topology);

    m_update_layout =
        std::make_unique<update_layout_service>(cmds, g, m_layout, m_topology);
    m_update_topology = std::make_unique<update_topology_service>(
        cmds, g, m_layout, m_topology);

    m_view = std::make_unique<view>(scene);

    m_presenter = std::make_unique<core_presenter>(
        g, *m_view, *m_update_layout, *m_update_topology);

    m_presenter->update_view(*m_layout, *m_topology);
}

} // namespace features::layout