#include "core.hpp"

#include "layout_factory.hpp"
#include "topology_factory.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

void core::initialize(command_history& cmds,
                      const std::string& layout_type,
                      const std::string& topology_type,
                      double topology_scale,
                      const graph& g,
                      const Ogre::SceneManager& scene)
{
    initialize_topology(topology_type, topology_scale);
    initialize_layout(layout_type, g);
    initialize_mvp(cmds, g, scene);

    assert(m_presenter);
    m_presenter->update_view(*m_layout);
}

void core::initialize_topology(const std::string& topology_type, double scale)
{
    m_space = topology_factory::make_topology(topology_type, scale);
}

void core::initialize_layout(const std::string& layout_type, const graph& g)
{
    m_layout = layout_factory::make_layout(layout_type, g, m_space);
}

void core::initialize_mvp(command_history& cmds,
                          const graph& g,
                          const Ogre::SceneManager& scene)
{
    m_view = std::make_unique<view>(m_pipeline, scene);

    m_presenter = std::make_unique<presenter>(
        m_pipeline, g, [&](const auto& data) { m_view->draw(data); });

    m_controller = std::make_unique<controller>(
        m_pipeline,
        [&](const auto& e) {
            cmds.execute(std::make_unique<update_layout_service>(
                m_pipeline,
                e,
                g,
                m_space,
                m_layout,
                layout_factory::make_layout));
        },
        [&](const auto& e) {
            cmds.execute(std::make_unique<update_topology_service>(
                m_pipeline,
                e,
                g,
                m_space,
                m_layout,
                topology_factory::make_topology,
                layout_factory::make_layout));
        });
}

} // namespace visualization::layout