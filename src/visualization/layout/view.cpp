#include "view.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

ogre_view::ogre_view(event_bus& pipeline, const Ogre::SceneManager& scene)
    : m_pipeline{pipeline}, m_scene{scene}
{}

void ogre_view::draw(const layout_data& data)
{
    draw_vertices(data);
    draw_edges(data);
}

void ogre_view::draw_vertices(const layout_data& data)
{
    for (const auto& [id, pos] : data)
    {
        assert(m_scene.hasSceneNode(id));

        m_scene.getSceneNode(id)->setPosition(pos.x, pos.y, pos.z);

        BOOST_LOG_TRIVIAL(debug) << "vertex: " << id << " drawn at: (" << pos.x
                                 << ", " << pos.y << ", " << pos.z << ')';
    }
}

// TODO
void ogre_view::draw_edges(const layout_data&) {}

} // namespace visualization::layout