// Contains the main architecture graph interface.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_GRAPH_INTERFACE_HPP
#define PRESENTATION_GRAPH_INTERFACE_HPP

#include "def.hpp" // for all the modules

namespace presentation
{

/***********************************************************
 * Graph Interface                                         *
 ***********************************************************/

// Combines all of the graph property management backend systems into a facade.
class graph_interface
{
public:
    graph_interface(
        symbol_table st,
        graph&& g, // NOTE: Doesn't support efficient move construction.
        vertex_marker m,
        weights_config w_cfg,
        layout_config l_cfg,
        scaling_config scaling_cfg,
        clustering_config clustering_cfg,
        color_coding_config col_cfg);

    auto get_symbol_table() const -> const auto& { return m_st; }
    auto get_graph() const -> const auto& { return m_g; }
    auto get_vertex_marker() const -> const auto& { return m_marker; }

    auto get_weights_backend() const -> const auto& { return m_weights; }
    auto get_weights_backend() -> auto& { return m_weights; }

    auto get_layout_backend() const -> const auto& { return m_layout; }
    auto get_layout_backend() -> auto& { return m_layout; }

    auto get_scaling_backend() const -> const auto& { return m_scaling; }
    auto get_scaling_backend() -> auto& { return m_scaling; }

    auto get_clustering_backend() const -> const auto& { return m_clustering; }
    auto get_clustering_backend() -> auto& { return m_clustering; }

    auto get_color_coding_backend() const -> const auto& { return m_cols; }
    auto get_color_coding_backend() -> auto& { return m_cols; }

private:
    symbol_table m_st;
    graph m_g;
    vertex_marker m_marker;
    weights_backend m_weights;
    layout_backend m_layout;
    scaling_backend m_scaling;
    clustering_backend m_clustering;
    color_coding_backend m_cols;
};

/***********************************************************
 * Functions                                               *
 ***********************************************************/

auto vertex_id(const graph_interface& g) -> id_map;
auto vertex_position(const graph_interface& g) -> position_map;
auto vertex_scale(const graph_interface& g) -> scale_map;
auto vertex_cluster(const graph_interface& g) -> cluster_map;

auto edge_dependency(const graph_interface&) -> dependency_map;
auto edge_weight(const graph_interface& g) -> weight_map;
auto edge_color(const graph_interface& g) -> color_map;

auto get_vertex(const graph_interface& g, const id_t& id) -> vertex;

} // namespace presentation

#endif // PRESENTATION_GRAPH_INTERFACE_HPP
