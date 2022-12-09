#include "camera.hpp"

#include "graph_interface.hpp"

#include <OGRE/OgreSceneNode.h>

namespace presentation
{

auto pan_at(const graph_interface& g, const id_t& id, node_t& cam) -> void
{
    using namespace Ogre;

    const bool vertex_exists = (g.get_symbol_table().lookup(id) != nullptr);
    if (vertex_exists)
    {
        const auto vert = get_vertex(g, id);
        const auto [x, y, z] = boost::get(vertex_position(g), vert);
        const auto vec = Vector3(x, y, z);
        cam.setPosition(vec + Vector3(0, 0, 200));
        cam.lookAt(vec, Node::TransformSpace::TS_WORLD);
    }
}

} // namespace presentation