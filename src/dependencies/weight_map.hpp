// Contains a function for creating a dynamic managed graph weight map.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_MAP_HPP
#define DEPENDENCIES_WEIGHT_MAP_HPP

#include "detail/weight_map.hpp"
#include "weight_repo.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map/function_property_map.hpp>

namespace dependencies
{

// A runtime managed edge-weight property map from a weight repository.
template < typename Graph, typename DependencyMap >
using dynamic_weight_map = boost::function_property_map<
    detail::weight_dispatcher< Graph, DependencyMap >,
    typename boost::graph_traits< Graph >::edge_descriptor,
    weight_repo::weight_type >;

// Creates a runtime managed edge-weight property map from a weight repository.
template < typename Graph, typename DependencyMap >
inline auto
make_dynamic_weight_map(const weight_repo& repo, DependencyMap edge_dependency)
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            DependencyMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< DependencyMap >::value_type,
                  weight_repo::dependency_type >);

    return dynamic_weight_map< Graph, DependencyMap >(
        detail::weight_dispatcher< Graph, DependencyMap >(
            repo, edge_dependency));
}

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_MAP_HPP
