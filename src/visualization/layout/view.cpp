#include "view.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

view::view(event_bus& pipeline, const Ogre::SceneManager& scene)
    : m_pipeline{pipeline}, m_scene{scene}
{}

void view::draw_vertex(const std::string& id,
                       double x,
                       double y,
                       double z) const
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex: " << id << " drawn at: (" << x << ", "
                             << y << ", " << z << ')';
}

} // namespace visualization::layout