// Contains the main architecture graph interface.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_INTERFACE_HPP
#define ARCHITECTURE_GRAPH_INTERFACE_HPP

#include "clustering/backend.hpp"
#include "clustering/cluster_map.hpp"
#include "graph.hpp"
#include "layout/position_map.hpp"
#include "metadata.hpp"
#include "scaling/scale_map.hpp"
#include "symbol_table.hpp"
#include "weights/weight_map.hpp"

namespace architecture
{

// Combines all of the graph property management backend systems into a facade.
class graph_interface
{
public:
    using weight_map = weights::weight_map< graph, dependency_map >;
    using position_map = layout::position_map< graph, weight_map >;
    using scale_map = scaling::scale_map< graph, metadata_counter >;

    using weights_backend_type = weights::backend;
    using weights_config_type = weights::backend::config_data_type;
    using layout_backend_type = layout::backend< graph, weight_map >;
    using layout_config_type = typename layout_backend_type::config_data_type;
    using scaling_backend_type = scaling::backend;
    using scaling_config_type = scaling_backend_type::config_data_type;
    using clustering_backend_type = clustering::backend< graph, weight_map >;
    using clustering_config_type = clustering::backend_config;

    using cluster_map = clustering::cluster_map< clustering_backend_type >;

    using dependency_type = weights_backend_type::dependency_type;
    using weight_type = weights_backend_type::weight_type;

    using layout_id_type = typename layout_backend_type::id_type;
    using topology_id_type = typename layout_backend_type::id_type;

    using scale_type = typename layout_backend_type::scale_type;
    using scaling_tag_type = scaling_backend_type::tag_type;
    using scaling_baseline_type = scaling_backend_type::baseline_type;
    using scaling_dims_type = scaling_backend_type::dims_type;
    using scaling_enabled_type = scaling_backend_type::enabled_type;
    using scaling_ratio_type = scaling_backend_type::ratio_type;

    using clusterer_id_type = typename clustering_backend_type::id_type;
    using mst_finder_id_type = typename clustering_backend_type::id_type;
    using k_type = typename clustering_backend_type::k_type;
    using snn_thres_type = typename clustering_backend_type::snn_threshold_type;
    using modularity_type = typename clustering_backend_type::modularity_type;
    using gamma_type = typename clustering_backend_type::gamma_type;
    using steps_type = typename clustering_backend_type::steps_type;

    graph_interface(
        symbol_table st,
        graph&& g,
        weights_config_type weights_cfg,
        layout_config_type layout_cfg,
        scaling_config_type scaling_cfg,
        clustering_config_type clustering_cfg);

    auto get_symbol_table() const -> const symbol_table& { return m_st; }
    auto get_graph() const -> const graph& { return m_g; }

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
    symbol_table m_st;
    graph m_g;
    weights_backend_type m_weights;
    layout_backend_type m_layout;
    scaling_backend_type m_scaling;
    clustering_backend_type m_clustering;
};

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_INTERFACE_HPP
