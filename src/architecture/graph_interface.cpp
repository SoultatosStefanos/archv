#include "graph_interface.hpp"

namespace architecture
{

graph_interface::graph_interface(
    symbol_table st,
    graph&& g,
    weights_config_type weights_cfg,
    layout_config_type layout_cfg,
    scaling_config_type scaling_cfg,
    clustering_config_type clustering_cfg)
: m_st { std::move(st) }
, m_g { std::move(g) }
, m_weights { std::move(weights_cfg) }
, m_layout { m_g, edge_weight(), std::move(layout_cfg) }
, m_scaling { std::move(scaling_cfg) }
, m_clustering { m_g, edge_weight(), std::move(clustering_cfg) }
{
}

auto graph_interface::vertex_id() const -> id_map
{
    return boost::get(boost::vertex_bundle, get_graph());
}

auto graph_interface::vertex_position() const -> position_map
{
    return layout::make_position_map(layout_backend());
}

auto graph_interface::vertex_scale() const -> scale_map
{
    return scaling::make_scale_map< graph >(
        scaling_backend(), metadata_counter(get_symbol_table(), get_graph()));
}

auto graph_interface::vertex_cluster() const -> cluster_map
{
    return clustering::make_cluster_map(clustering_backend());
}

auto graph_interface::edge_dependency() const -> dependency_map
{
    return boost::get(boost::edge_bundle, get_graph());
}

auto graph_interface::edge_weight() const -> weight_map
{
    return weights::make_weight_map< graph >(
        weights_backend(), edge_dependency());
}

} // namespace architecture