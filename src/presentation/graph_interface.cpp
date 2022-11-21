#include "graph_interface.hpp"

namespace presentation
{

graph_interface::graph_interface(
    symbol_table_type st,
    graph_type&& g,
    weights_backend_type::config_data_type weights_cfg,
    layout_backend_type::config_data_type layout_cfg,
    scaling_backend_type::config_data_type scaling_cfg,
    clustering_backend_type::config_data_type clustering_cfg)
: m_st { std::move(st) }
, m_g { std::move(g) }
, m_weights { std::move(weights_cfg) }
, m_layout { m_g, edge_weight(*this), std::move(layout_cfg) }
, m_scaling { std::move(scaling_cfg) }
, m_clustering { m_g, edge_weight(*this), std::move(clustering_cfg) }
{
}

auto vertex_id(const graph_interface& g) -> graph_interface::id_map
{
    return boost::get(boost::vertex_bundle, g.graph());
}

auto vertex_position(const graph_interface& g) -> graph_interface::position_map
{
    return layout::make_position_map(g.layout_backend());
}

auto vertex_scale(const graph_interface& g) -> graph_interface::scale_map
{
    return scaling::make_scale_map< graph_interface::graph_type >(
        g.scaling_backend(),
        architecture::metadata_counter(g.symbol_table(), g.graph()));
}

auto vertex_cluster(const graph_interface& g) -> graph_interface::cluster_map
{
    return clustering::make_cluster_map(g.clustering_backend());
}

auto edge_dependency(const graph_interface& g)
    -> graph_interface::dependency_map
{
    return boost::get(boost::edge_bundle, g.graph());
}

auto edge_weight(const graph_interface& g) -> graph_interface::weight_map
{
    return weights::make_weight_map< graph_interface::graph_type >(
        g.weights_backend(), edge_dependency(g));
}

} // namespace presentation