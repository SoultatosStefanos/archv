// Contains a function for creating a dynamic managed graph weight map.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_MAP_HPP
#define DEPENDENCIES_WEIGHT_MAP_HPP

#include "weight_repo.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace dependencies
{

namespace detail
{
    template <typename Graph, typename DependencyMap>
    class weight_dispatcher
    {
    public:
        using graph = Graph;
        using dependency_map = DependencyMap;
        using edge_type = typename boost::graph_traits<graph>::edge_descriptor;
        using weight = weight_repo::weight;

        weight_dispatcher(const weight_repo& repo,
                          dependency_map edge_dependency)
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

} // namespace detail

// A runtime managed edge-weight property map from a weight repository.
template <typename Graph, typename DependencyMap>
using dynamic_weight_map = boost::function_property_map<
    detail::weight_dispatcher<Graph, DependencyMap>,
    typename boost::graph_traits<Graph>::edge_descriptor,
    weight_repo::weight>;

// Creates a runtime managed edge-weight property map from a weight repository.
template <typename Graph, typename DependencyMap>
inline auto make_dynamic_weight_map(const weight_repo& repo,
                                    DependencyMap edge_dependency)
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            DependencyMap,
            typename boost::graph_traits<Graph>::edge_descriptor>) );

    static_assert(std::is_convertible_v<
                  typename boost::property_traits<DependencyMap>::value_type,
                  weight_repo::dependency_type>);

    return dynamic_weight_map<Graph, DependencyMap>(
        detail::weight_dispatcher<Graph, DependencyMap>(repo, edge_dependency));
}

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_MAP_HPP
