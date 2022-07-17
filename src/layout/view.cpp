#include "view.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout
{

view::view(const Ogre::SceneManager& scene) : m_scene{scene} {}

void view::draw_vertex(const vertex_id& id, coord x, coord y, coord z)
{
    assert(m_scene.hasSceneNode(id));

    m_scene.getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex: " << id << " drawn at: (" << x << ", "
                             << y << ", " << z << ')';
}

// TODO
void view::update_layout_selection(layout_selection)
{
    BOOST_LOG_TRIVIAL(error) << "update_layout_selection() not implemented yet";
}

// TODO
void view::update_topology_selection(topology_selection,
                                     topology_scale_selection)
{
    BOOST_LOG_TRIVIAL(error)
        << "update_topology_selection() not implemented yet";
}

void view::on_layout_request(layout_request_listener f)
{
    m_layout_signal.connect(f);
}

void view::on_topology_request(topology_request_listener f)
{
    m_topology_signal.connect(f);
}

void view::send_layout_request(layout_selection l) const
{
    BOOST_LOG_TRIVIAL(info) << "layout selected with type: " << l;

    m_layout_signal(l);
}

void view::send_topology_request(topology_selection space,
                                 topology_scale_selection scale) const
{
    BOOST_LOG_TRIVIAL(info)
        << "topology selected with type: " << space << " and scale: " << scale;

    m_topology_signal(space, scale);
}

} // namespace layout