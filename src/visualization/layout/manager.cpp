#include "manager.hpp"

#include "topology_factory.hpp"

namespace visualization
{

void layout_manager::initialize(const graph& g, const Ogre::SceneManager& scene)
{
    m_service = std::make_unique<layout_service>(g);
    m_view = std::make_unique<layout_view>(scene);
    m_controller = std::make_unique<layout_controller<>>(*m_view, *m_service);
    m_presenter = std::make_unique<layout_presenter<>>(g, *m_view, *m_service);
}

void layout_manager::reset()
{
    m_service.reset();
    m_view.reset();
    m_controller.reset();
    m_presenter.reset();
}

} // namespace visualization