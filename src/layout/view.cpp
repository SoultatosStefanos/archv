#include "view.hpp"

#include <cassert>

namespace layout
{

view::view(const Ogre::SceneManager& scene) : m_scene{scene} {}

void view::draw_vertex(const std::string& id, double x, double y, double z)
{
    assert(m_scene.hasSceneNode(id));

    m_scene.getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex: " << id << " drawn at: (" << x << ", "
                             << y << ", " << z << ')';
}

void view::update_layout_selection(std::string l)
{
    BOOST_LOG_TRIVIAL(info) << "view layout selection updated to: " << l;
    BOOST_LOG_TRIVIAL(error) << "update_layout_selection() not implemented yet";
    [[maybe_unused]] auto _ = std::move(l);
}

void view::update_topology_selection(std::string s, double scale)
{
    BOOST_LOG_TRIVIAL(info) << "view topology selection updated to: " << s
                            << ", with scale: " << scale;
    BOOST_LOG_TRIVIAL(error)
        << "update_topology_selection() not implemented yet";

    [[maybe_unused]] auto _ = std::move(s);
}

} // namespace layout