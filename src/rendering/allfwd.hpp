// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef RENDERING_ALLFWD_HPP
#define RENDERING_ALLFWD_HPP

namespace rendering
{

struct background_config;
class background_config_api;

class background_renderer;

template < typename Graph, typename VertexID, typename DependencyMap >
class graph_renderer;

template < typename Renderer, typename View >
class background_presenter;

} // namespace rendering

#endif // RENDERING_ALLFWD_HPP
