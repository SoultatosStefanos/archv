#ifndef METADATA_GRAPH_ACCESSOR_HPP
#define METADATA_GRAPH_ACCESSOR_HPP

#include "config.hpp"
#include "dependencies/backend.hpp"
#include "graph.hpp"
#include "layout/backend.hpp"

namespace metadata
{

// Provides an interface for accessing any graph property.
class graph_accessor
{
public:
    using graph_type = graph;
    using vertex_type = graph_type::vertex_descriptor;
    using dependencies_config_type = dependencies::backend::config_data_type;
    using layout_config_type = layout::backend_config;

    graph_accessor(
        dependencies_config_type dependencies_config,
        layout_config_type layout_config);

    auto dependencies_backend() const -> const auto& { }
    auto dependencies_backend() -> auto& { }

    auto layout_backend() const -> const auto& { }
    auto layout_backend() -> auto& { }

    auto vertex(const std::string& id) const -> vertex_type { }
    auto weight_map(vertex_type v) const -> auto { }
    auto position_map(vertex_type v) const -> auto { }

private:
};

} // namespace metadata

#endif // METADATA_GRAPH_ACCESSOR_HPP
