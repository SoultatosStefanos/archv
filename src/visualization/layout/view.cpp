#include "view.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

view::view(event_bus& pipeline, const Ogre::SceneManager& scene)
    : m_pipeline{pipeline}, m_scene{scene}
{}

void view::draw(const layout_data& data)
{
    draw_vertices(data);
    draw_edges(data);
}

void view::draw_vertices(const layout_data& data)
{
    for (const auto& [id, pos] : data)
    {
        assert(m_scene.hasSceneNode(id));

        const auto& [x, y, z] = pos;
        m_scene.getSceneNode(id)->setPosition(x, y, z);

        BOOST_LOG_TRIVIAL(debug) << "vertex: " << id << " drawn at: (" << x
                                 << ", " << y << ", " << z << ')';
    }
}

// TODO
void view::draw_edges(const layout_data&) {}

} // namespace visualization::layout