// Contains a private subcomponent of the weight_map.hpp module.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_DETAIL_WEIGHT_MAP_HPP
#define DEPENDENCIES_DETAIL_WEIGHT_MAP_HPP

#include "dependencies/backend.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace dependencies::detail
{

template < typename Graph, typename DependencyMap >
class weight_dispatcher
{
public:
    using graph_type = Graph;
    using dependency_map = DependencyMap;
    using edge_type = typename graph_type ::edge_descriptor;
    using weight_type = backend::weight_type;

    weight_dispatcher(const backend& b, dependency_map edge_dependency)
    : m_backend { &b }, m_edge_dependency { edge_dependency }
    {
    }

    auto operator()(edge_type e) const -> weight_type
    {
        const auto& repo = m_backend->get_weight_repo();
        const auto dependency = boost::get(m_edge_dependency, e);
        return repo.get_weight(dependency);
    }

private:
    const backend* m_backend { nullptr };
    dependency_map m_edge_dependency;
};

} // namespace dependencies::detail

#endif // DEPENDENCIES_DETAIL_WEIGHT_MAP_HPP
