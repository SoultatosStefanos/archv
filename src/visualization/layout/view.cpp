#include "view.hpp"

namespace visualization::layout
{

void view::draw_vertex(const std::string& id,
                       double x,
                       double y,
                       double z) const
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setPosition(x, y, z);
    m_pipeline.post(vertex_drawn_event{.id = id, .x = x, .y = y, .z = z});
}

} // namespace visualization::layout