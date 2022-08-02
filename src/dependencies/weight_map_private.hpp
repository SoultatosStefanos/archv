// Contains a private subcomponent of the weight_map.hpp module.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_MAP_PRIVATE_HPP
#define DEPENDENCIES_WEIGHT_MAP_PRIVATE_HPP

#include "weight_repo.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace dependencies::detail
{

template <typename Graph, typename DependencyMap>
class weight_dispatcher
{
public:
    using graph = Graph;
    using dependency_map = DependencyMap;
    using edge_type = typename boost::graph_traits<graph>::edge_descriptor;
    using weight = weight_repo::weight;

    weight_dispatcher(const weight_repo& repo, dependency_map edge_dependency)
        : m_repo{repo}, m_edge_dependency{edge_dependency}
    {}

    auto operator()(edge_type e) const -> weight
    {
        const auto& dependency = boost::get(m_edge_dependency, e);
        return m_repo.get_weight(dependency);
    }

private:
    const weight_repo& m_repo;
    dependency_map m_edge_dependency;
};

} // namespace dependencies::detail

#endif // DEPENDENCIES_WEIGHT_MAP_PRIVATE_HPP
