#include "graph_interface.hpp"

namespace application
{

graph_interface::graph_interface(
    symbol_table_type st,
    graph_type g,
    dependencies_config_type dependencies_cfg,
    layout_config_type layout_cfg,
    scaling_config_type scaling_cfg)
: m_st { std::move(st) }
, m_g { std::move(g) }
, m_dependencies { std::move(dependencies_cfg) }
, m_layout { m_g, edge_weight(), std::move(layout_cfg) }
, m_scaling { std::move(scaling_cfg) }
{
}

auto graph_interface::vertex_id() const -> id_map
{
    return boost::get(boost::vertex_bundle, std::as_const(graph()));
}

auto graph_interface::vertex_position() const -> position_map
{
    return layout::make_position_map(layout_backend());
}

auto graph_interface::vertex_scale() const -> scale_map
{
    return scaling::make_scale_map< graph_type >(
        scaling_backend(), metadata_counter_type(symbol_table(), graph()));
}

auto graph_interface::edge_dependency() const -> dependency_map
{
    return boost::get(boost::edge_bundle, std::as_const(graph()));
}

auto graph_interface::edge_weight() const -> weight_map
{
    return dependencies::make_weight_map< graph_type >(
        dependencies_backend(), edge_dependency());
}

auto make_graph_interface(std::string_view graph_file_path) -> graph_interface
{
    const auto& archive = config::archive::get();
    const auto& arch_root = archive.at(graph_file_path);
    const auto& deps_root = archive.at(ARCHV_DEPS_CONFIG_PATH);
    const auto& layout_root = archive.at(ARCHV_LAYOUT_CONFIG_PATH);
    const auto& scale_root = archive.at(ARCHV_SCALING_CONFIG_PATH);

    auto&& deps_cfg = dependencies::deserialize(deps_root);
    auto&& layout_cfg = layout::deserialize(layout_root);
    auto&& scale_cfg = scaling::deserialize(scale_root);
    auto&& [symbol_table, graph, _] = architecture::deserialize(arch_root);

    return graph_interface(
        std::move(symbol_table),
        std::move(graph),
        std::move(deps_cfg),
        std::move(layout_cfg),
        std::move(scale_cfg));
}

} // namespace application