// Contains a private module for the color map header.
// Soultatos Stefanos 2022

#ifndef COLOR_CODING_DETAIL_COLOR_MAP_HPP
#define COLOR_CODING_DETAIL_COLOR_MAP_HPP

#include "color_coding/backend.hpp" // for backend

#include <boost/graph/graph_concepts.hpp> // for boost::get
#include <optional>                       // for optional

namespace color_coding::detail
{

template < typename DependencyMap >
struct color_dispatcher
{
    using backend_type = backend;
    using dependency_map_type = DependencyMap;
    using color_code_type = backend_type::color_code_type;
    using rgba_type = backend_type::rgba_type;
    using result_type = std::optional< rgba_type >;

    const backend_type* back { nullptr };
    dependency_map_type edge_dependency;

    template < typename Edge >
    inline auto operator()(Edge e) const -> result_type
    {
        const auto& dependency = boost::get(edge_dependency, e);
        const auto& [col, active] = get_color_code(*back, dependency);
        return active ? result_type(col) : std::nullopt;
    }
};

} // namespace color_coding::detail

#endif // COLOR_CODING_DETAIL_COLOR_MAP_HPP
