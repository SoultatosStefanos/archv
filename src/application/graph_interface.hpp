// Contains a class which combines all of the graph backends and acts as an
// access layer to all of the architecture graph properties.
// Soultatos Stefanos 2022

#ifndef APPLICATION_GRAPH_INTERFACE_HPP
#define APPLICATION_GRAPH_INTERFACE_HPP

#include "architecture/all.hpp"
#include "config/all.hpp"
#include "dependencies/all.hpp"
#include "layout/all.hpp"
#include "scaling/all.hpp"

#include <string_view>

namespace application
{

// TODO: Pass commands, add functions here
class graph_interface
{
public:
    using symbol_table_type = architecture::symbol_table;
    using graph_type = architecture::graph;
    using metadata_counter_type = architecture::metadata_counter;

    using id_map = architecture::id_map;
    using dependency_map = architecture::dependency_map;
    using weight_map = dependencies::weight_map< graph_type, dependency_map >;
    using position_map = layout::position_map< graph_type, weight_map >;
    using scale_map = scaling::scale_map< graph_type, metadata_counter_type >;

    using dependencies_backend_type = dependencies::backend;
    using dependencies_config_type = dependencies::backend::config_data_type;
    using layout_backend_type = layout::backend< graph_type, weight_map >;
    using layout_config_type = layout_backend_type::config_data_type;
    using scaling_backend_type = scaling::backend;
    using scaling_config_type = scaling_backend_type::config_data_type;

    graph_interface(
        symbol_table_type st,
        graph_type g,
        dependencies_config_type dependencies_cfg,
        layout_config_type layout_cfg,
        scaling_config_type scaling_cfg);

    auto symbol_table() const -> const symbol_table_type& { return m_st; }
    auto symbol_table() -> symbol_table_type& { return m_st; }

    auto graph() const -> const graph_type& { return m_g; }
    auto graph() -> graph_type& { return m_g; }

    auto dependencies_backend() const -> const auto& { return m_dependencies; }
    auto dependencies_backend() -> auto& { return m_dependencies; }

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
    symbol_table_type m_st;
    graph_type m_g;
    dependencies_backend_type m_dependencies;
    layout_backend_type m_layout;
    scaling_backend_type m_scaling;
};

// From a graph .json file.
auto make_graph_interface(std::string_view graph_file_path) -> graph_interface;

} // namespace application

#endif // APPLICATION_GRAPH_INTERFACE_HPP
