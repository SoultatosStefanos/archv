#include "view.hpp"

#include <boost/log/trivial.hpp>

namespace visualization
{

void layout_view::draw_vertex(const std::string& id,
                              double x,
                              double y,
                              double z) const
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug)
        << "drew vertex at: (" << x << ", " << y << ", " << z << ')';
}

void layout_view::add_layout_input_listener(on_layout_input f)
{
    m_layout_listeners.push_back(std::move(f));
}

void layout_view::add_topology_input_listener(on_topology_input f)
{
    m_topology_listeners.push_back(std::move(f));
}

void layout_view::notify_layout_input(const std::string& type) const
{
    for (const auto& f : m_layout_listeners)
        f(type);
}

void layout_view::notify_topology_input(const std::string& type,
                                        double scale) const
{
    for (const auto& f : m_topology_listeners)
        f(type, scale);
}

} // namespace visualization