#include "graph_interface.hpp"

namespace presentation
{

graph_interface::graph_interface(
    symbol_table st,
    graph&& g,
    vertex_marker m,
    weights_config w_cfg,
    layout_config l_cfg,
    scaling_config scaling_cfg,
    clustering_config clus_cfg,
    color_coding_config col_cfg,
    degrees_config deg_cfg)
: m_st { std::move(st) }
, m_g { std::move(g) }
, m_marker { std::move(m) }
, m_weights { std::move(w_cfg) }
, m_layout { m_g, edge_weight(*this), std::move(l_cfg) }
, m_scaling { std::move(scaling_cfg) }
, m_clustering { m_g, edge_weight(*this), std::move(clus_cfg) }
, m_cols { std::move(col_cfg) }
, m_degrees { std::move(deg_cfg) }
{
}

auto vertex_id(const graph_interface& g) -> id_map
{
    return boost::get(boost::vertex_bundle, g.get_graph());
}

auto vertex_position(const graph_interface& g) -> position_map
{
    return layout::make_position_map(g.get_layout_backend());
}

auto vertex_scale(const graph_interface& g) -> scale_map
{
    return scaling::make_scale_map< graph >(
        g.get_scaling_backend(),
        metadata_counter(g.get_symbol_table(), g.get_graph()));
}

auto vertex_cluster(const graph_interface& g) -> cluster_map
{
    return clustering::make_cluster_map(g.get_clustering_backend());
}

auto edge_dependency(const graph_interface& g) -> dependency_map
{
    return boost::get(boost::edge_bundle, g.get_graph());
}

auto edge_weight(const graph_interface& g) -> weight_map
{
    return weights::make_weight_map< graph >(
        g.get_weights_backend(), edge_dependency(g));
}

auto edge_color(const graph_interface& g) -> color_map
{
    return color_coding::make_color_map< graph >(
        g.get_color_coding_backend(), edge_dependency(g));
}

auto get_vertex(const graph_interface& g, const id_t& id) -> vertex
{
    return g.get_vertex_marker().vertex(id);
}

} // namespace presentation