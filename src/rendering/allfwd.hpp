// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef RENDERING_ALLFWD_HPP
#define RENDERING_ALLFWD_HPP

namespace rendering
{

class background_renderer;

template <
    typename Graph,
    typename VertexID,
    typename WeightMap,
    typename PositionMap >
class graph_renderer;

} // namespace rendering

#endif // RENDERING_ALLFWD_HPP
