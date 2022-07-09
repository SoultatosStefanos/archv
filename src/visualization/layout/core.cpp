#include "core.hpp"

#include "layout_factory.hpp"
#include "topology_factory.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

void core::initialize(const std::string& layout_type,
                      const std::string& topology_type,
                      double scale,
                      const graph& g,
                      const Ogre::SceneManager& scene)
{
    initialize_topology(topology_type, scale);
    initialize_layout(layout_type, g);
    hook_mvp(layout_type, topology_type, scale, g, scene);
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

void core::hook_mvp(const std::string& layout_type,
                    const std::string& topology_type,
                    double scale,
                    const graph& g,
                    const Ogre::SceneManager& scene)
{
    m_update_layout = std::make_unique<update_layout_service>(
        m_pipeline, g, m_layout, layout_type, &m_space);

    m_update_topology = std::make_unique<update_topology_service>(
        m_pipeline, g, m_space, topology_type, scale);

    m_view = std::make_unique<view>(m_pipeline, scene);

    m_controller = std::make_unique<controller>(
        m_pipeline,
        [&](const auto& type) { m_update_layout->update(type); },
        [&](const auto& type, auto scale) {
            m_update_topology->update(type, scale);
        });

    m_presenter = std::make_unique<presenter>(
        m_pipeline, g, [&](const auto& id, auto x, auto y, auto z) {
            m_view->draw_vertex(id, x, y, z);
        });
}

} // namespace visualization::layout