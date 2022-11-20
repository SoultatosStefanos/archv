// Contains a utility class for labeling architecture graph vertices & accessing
// them in O(1) time.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_VERTEX_MARKER_HPP
#define ARCHITECTURE_VERTEX_MARKER_HPP

#include "graph.hpp"

#include <unordered_map>

namespace architecture
{

/***********************************************************
 * Vertex Marker                                           *
 ***********************************************************/

class vertex_marker
{
public:
    using graph_type = graph;
    using id_type = structure::id_type;
    using vertex_type = boost::graph_traits< graph >::vertex_descriptor;
    using size_type = std::size_t;

    auto vertex(const id_type& id) const -> vertex_type;

    auto num_marked() const -> size_type;
    auto marks(const id_type& id) const -> bool;

    auto mark(id_type id, vertex_type v) -> void;

private:
    using map = std::unordered_map< id_type, vertex_type >;

    map m_map;
};

} // namespace architecture

#endif // ARCHITECTURE_VERTEX_MARKER_HPP
