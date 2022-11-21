#include "camera.hpp"

#include "graph_interface.hpp"

namespace presentation
{

auto pan_at(const graph_interface& g, const id_t& id, node_t& cam) -> void
{
    using namespace Ogre;

    const bool vertex_exists = (g.symbol_table().lookup(id) != nullptr);
    if (vertex_exists)
    {
        const auto [x, y, z] = vertex_position(g, id);
        const auto vec = Vector3(x, y, z);
        const auto rel = Node::TransformSpace::TS_WORLD;
        cam.lookAt(vec, rel);
    }
}

} // namespace presentation