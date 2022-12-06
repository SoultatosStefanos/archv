// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef RENDERING_ALLFWD_HPP
#define RENDERING_ALLFWD_HPP

#include "concepts.hpp"

namespace rendering
{

struct background_config;
class background_config_api;

class background_renderer;

struct graph_config;
struct graph_config_api;

template <
    typename Graph,
    typename VertexID,
    typename DependencyMap,
    degrees_evaluator DegreesEvaluator,
    cluster_color_coder ClusterColorCoder >
class graph_renderer;

template < typename Graph, typename VertexID, typename DependencyMap >
class graph_collision_checker;

class degrees_ranked_evaluator;
class degrees_ranked_backend;

struct cluster_color_pool;

struct minimap_config;
class minimap_config_api;
class minimap_renderer;

struct config_data;

} // namespace rendering

#endif // RENDERING_ALLFWD_HPP
