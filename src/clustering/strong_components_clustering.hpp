// Contains a generic Strong Components clustering algorithm
// Soultatos Stefanos 2022

#ifndef CLUSTERING_STRONG_COMPONENTS_CLUSTERING_HPP
#define CLUSTERING_STRONG_COMPONENTS_CLUSTERING_HPP

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <cassert>
#include <vector>

namespace Clustering
{

namespace Details
{
    template <typename MutableGraph, typename ComponentMap>
    void strong_components_clustering_impl(MutableGraph& g, ComponentMap comp)
    {
        BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<MutableGraph>) );
        BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<MutableGraph>) );
        BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );
        BOOST_CONCEPT_ASSERT((boost::WritablePropertyMapConcept<
                              ComponentMap,
                              typename boost::graph_traits<MutableGraph>::vertex_descriptor>) );

        static_assert(
            std::is_same_v<typename boost::property_traits<ComponentMap>::value_type, int>);

        if (boost::num_edges(g) == 0)
            return;

        [[maybe_unused]] const auto num = boost::strong_components(g, comp);

        boost::remove_edge_if(
            [&g, comp, num](auto e) {
                assert(boost::get(comp, boost::source(e, g)) < num);
                assert(boost::get(comp, boost::target(e, g)) < num);

                return boost::get(comp, boost::source(e, g)) !=
                       boost::get(comp, boost::target(e, g));
            },
            g);
    }

} // namespace Details

template <typename MutableGraph, typename ComponentMap>
inline void strong_components_clustering(MutableGraph& g, ComponentMap comp)
{
    Details::strong_components_clustering_impl(g, comp);
}

template <typename MutableGraph>
inline void strong_components_clustering(MutableGraph& g)
{
    using ComponentStorage = std::vector<int>;

    ComponentStorage c(boost::num_vertices(g));
    Details::strong_components_clustering_impl(
        g, boost::make_iterator_property_map(std::begin(c), boost::get(boost::vertex_index, g)));
}

} // namespace Clustering

#endif // CLUSTERING_STRONG_COMPONENTS_CLUSTERING_HPP
