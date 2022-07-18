#include "core.hpp"

namespace layout
{

void core::initialize(command_history& cmds,
                      const graph& g,
                      const Ogre::SceneManager& scene,
                      layout_type l,
                      topology_type t,
                      topology_scale s)
{
    m_topology = topology_factory::make_topology(t, s);
    m_layout = layout_factory::make_layout(l, g, *m_topology);

    auto&& usecase1 = update_layout_service(cmds, g, m_layout, m_topology);
    auto&& usecase2 = update_topology_service(cmds, g, m_layout, m_topology);
    auto&& ui = view(scene);

    m_presenter = std::make_unique<presenter_type>(
        g, std::move(ui), std::move(usecase1), std::move(usecase2));

    m_presenter->update_view(*m_layout, *m_topology);
}

} // namespace layout