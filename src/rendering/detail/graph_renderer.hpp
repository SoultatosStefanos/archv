// Contains a private module used by the graph_renderer header.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_GRAPH_RENDERER_HPP
#define RENDERING_DETAIL_GRAPH_RENDERER_HPP

#include <OGRE/OgreVector3.h>
#include <boost/graph/graph_concepts.hpp>
#include <type_traits>

namespace rendering::detail
{

template <
    typename Graph,
    typename VertexID,
    typename WeightMap,
    typename PositionMap >
struct GraphRendererConcept
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            VertexID,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            PositionMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< VertexID >::value_type,
                  std::string >);

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< WeightMap >::value_type,
                  double >);

    static_assert(
        std::is_same_v <
            typename boost::property_traits< PositionMap >::value_type,
        Ogre::Vector3);
};

} // namespace rendering::detail

#endif // RENDERING_DETAIL_GRAPH_RENDERER_HPP
