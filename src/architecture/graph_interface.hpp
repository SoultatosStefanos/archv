// Contains the main architecture graph interface.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_INTERFACE_HPP
#define ARCHITECTURE_GRAPH_INTERFACE_HPP

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
    using layout_config_type = layout_backend_type::config_data_type;
    using scaling_backend_type = scaling::backend;
    using scaling_config_type = scaling_backend_type::config_data_type;

    graph_interface(
        symbol_table st,
        graph g,
        weights_config_type weights_cfg,
        layout_config_type layout_cfg,
        scaling_config_type scaling_cfg);

    auto get_symbol_table() const -> const symbol_table& { return m_st; }
    auto get_graph() const -> const graph& { return m_g; }

    auto weights_backend() const -> const auto& { return m_weights; }
    auto weights_backend() -> auto& { return m_weights; }

    auto layout_backend() const -> const auto& { return m_layout; }
    auto layout_backend() -> auto& { return m_layout; }

    auto scaling_backend() const -> const auto& { return m_scaling; }
    auto scaling_backend() -> auto& { return m_scaling; }

    auto vertex_id() const -> id_map;
    auto vertex_position() const -> position_map;
    auto vertex_scale() const -> scale_map;
    auto edge_dependency() const -> dependency_map;
    auto edge_weight() const -> weight_map;

private:
    symbol_table m_st;
    graph m_g;
    weights_backend_type m_weights;
    layout_backend_type m_layout;
    scaling_backend_type m_scaling;
};

} // namespace architecture

#endif // ARCHITECTURE_GRAPH_INTERFACE_HPP
