// Contains the main architecture graph interface.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_GRAPH_INTERFACE_HPP
#define PRESENTATION_GRAPH_INTERFACE_HPP

#include "architecture/graph.hpp"
#include "architecture/metadata.hpp"
#include "architecture/symbol_table.hpp"
#include "clustering/backend.hpp"
#include "clustering/cluster_map.hpp"
#include "layout/position_map.hpp"
#include "scaling/scale_map.hpp"
#include "weights/weight_map.hpp"

namespace presentation
{

// Combines all of the graph property management backend systems into a facade.
class graph_interface
{
public:
    using symbol_table_type = architecture::symbol_table;
    using graph_type = architecture::graph;

    using id_map = architecture::id_map;
    using dependency_map = architecture::dependency_map;
    using weight_map = weights::weight_map< graph_type, dependency_map >;
    using position_map = layout::position_map< graph_type, weight_map >;
    using scale_map
        = scaling::scale_map< graph_type, architecture::metadata_counter >;

    using weights_backend_type = weights::backend;
    using layout_backend_type = layout::backend< graph_type, weight_map >;
    using scaling_backend_type = scaling::backend;
    using clustering_backend_type
        = clustering::backend< graph_type, weight_map >;

    using cluster_map = clustering::cluster_map< clustering_backend_type >;

    graph_interface(
        symbol_table_type st,
        graph_type&& g,
        weights_backend_type::config_data_type weights_cfg,
        layout_backend_type::config_data_type layout_cfg,
        scaling_backend_type::config_data_type scaling_cfg,
        clustering_backend_type::config_data_type clustering_cfg);

    auto symbol_table() const -> const symbol_table_type& { return m_st; }
    auto graph() const -> const graph_type& { return m_g; }

    auto weights_backend() const -> const auto& { return m_weights; }
    auto weights_backend() -> auto& { return m_weights; }

    auto layout_backend() const -> const auto& { return m_layout; }
    auto layout_backend() -> auto& { return m_layout; }

    auto scaling_backend() const -> const auto& { return m_scaling; }
    auto scaling_backend() -> auto& { return m_scaling; }

    auto clustering_backend() const -> const auto& { return m_clustering; }
    auto clustering_backend() -> auto& { return m_clustering; }

    auto vertex_id() const -> id_map;
    auto vertex_position() const -> position_map;
    auto vertex_scale() const -> scale_map;
    auto vertex_cluster() const -> cluster_map;
    auto edge_dependency() const -> dependency_map;
    auto edge_weight() const -> weight_map;

private:
    symbol_table_type m_st;
    graph_type m_g;
    weights_backend_type m_weights;
    layout_backend_type m_layout;
    scaling_backend_type m_scaling;
    clustering_backend_type m_clustering;
};

} // namespace presentation

#endif // PRESENTATION_GRAPH_INTERFACE_HPP
