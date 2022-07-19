#include "core.hpp"

namespace layout
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

    auto&& usecase1 = update_layout_service(cmds, g, m_layout, m_topology);
    auto&& usecase2 = update_topology_service(cmds, g, m_layout, m_topology);
    auto&& ui = view(scene);

    m_presenter = std::make_unique<presenter_type>(
        g, std::move(ui), std::move(usecase1), std::move(usecase2));

    m_presenter->update_view(*m_layout, *m_topology);
}

} // namespace layout