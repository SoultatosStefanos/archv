// Contains the dynamic vertex scale property map of the application.
// Soultatos Stefanos 2022

#ifndef SCALING_SCALE_MAP_HPP
#define SCALING_SCALE_MAP_HPP

#include "backend.hpp"
#include "detail/scale_map.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <concepts>

namespace scaling
{

// A dynamic vertex scaling property map.
template < typename Graph, typename FactorCounter >
using scale_map = boost::function_property_map<
    detail::scale_dispatcher< Graph, FactorCounter >,
    typename boost::graph_traits< Graph >::vertex_descriptor,
    scale_vector >;

// Makes a dynamic vertex scaling property map.
// Convenience function.
template < typename Graph, typename FactorCounter >
requires std::invocable<
    FactorCounter,
    typename boost::graph_traits< Graph >::vertex_descriptor,
    factor_repo::hash_table::key_type >
inline auto make_scale_map(const backend& b, FactorCounter func)
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    return scale_map< Graph, FactorCounter >(
        detail::scale_dispatcher< Graph, FactorCounter >(b, std::move(func)));
}

} // namespace scaling

#endif // SCALING_SCALE_MAP_HPP
