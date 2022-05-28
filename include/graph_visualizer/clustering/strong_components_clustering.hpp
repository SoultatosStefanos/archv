// Contains a generic Strong Components clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "filtered_graph.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <cassert>

namespace GV::Clustering {

// Generic Strong Componenents Clustering algorithm, using boost::strong_components
// NOTE: Deliberately missing default component map overload
template <typename Graph, typename ComponentMap>
auto strong_components_clustering(const Graph& g, ComponentMap comp) -> Filtered<Graph>
{
    BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((boost::WritablePropertyMapConcept<
                          ComponentMap, typename boost::graph_traits<Graph>::vertex_descriptor>) );

    static_assert(std::is_same_v<typename boost::property_traits<ComponentMap>::value_type, int>);

    if (boost::num_edges(g) == 0) return make_filtered(g); // early exit

    [[maybe_unused]] const auto num = boost::strong_components(g, comp);

    return make_filtered(g, [&g, comp, num](auto edge) {
        assert(boost::get(comp, boost::source(edge, g)) < num);
        assert(boost::get(comp, boost::target(edge, g)) < num);
        // Keep edge if both src and target reside within the same component
        return boost::get(comp, boost::source(edge, g)) == boost::get(comp, boost::target(edge, g));
    });
}

} // namespace GV::Clustering