// Contains some utility functions for presenting the graph clustering.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_CLUSTERING_HPP
#define PRESENTATION_CLUSTERING_HPP

#include "deffwd.hpp" // for graph_renderer

namespace presentation
{

class graph_interface;

auto show_clusters(const graph_interface& g, graph_renderer& renderer) -> void;
auto hide_clusters(const graph_interface& g, graph_renderer& renderer) -> void;

} // namespace presentation

#endif // PRESENTATION_CLUSTERING_HPP
