// Contains a random layout implementation class.
// Soultatos Stefanos 2022

#ifndef LAYOUT_RANDOM_LAYOUT_HPP
#define LAYOUT_RANDOM_LAYOUT_HPP

#include "layout.hpp"   // for layout
#include "plugin.hpp"   // for random_id
#include "topology.hpp" // for topology

#include <boost/graph/adjacency_list.hpp> // for vertices
#include <boost/graph/random_layout.hpp>  // for random_layout
#include <cassert>                        // for assert

namespace layout
{

/***********************************************************
 * Random Layout                                           *
 ***********************************************************/

// This algorithm places the points of the graph at random locations within a
// given space.
//
// Implementation details:
// https://www.boost.org/doc/libs/1_52_0/libs/graph/doc/random_layout.html
//
// Wrapper over the boost implementation, for type erasure.

template < typename Graph >
class random_layout : public layout< Graph >
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    using base = layout< Graph >;
    using self = random_layout< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;

    random_layout(const graph_type& g, const topology& space);
    ~random_layout() override = default;

    auto id() const -> id_type override { return random_id; }

    auto x(vertex_type v) const -> coord_type override;
    auto y(vertex_type v) const -> coord_type override;
    auto z(vertex_type v) const -> coord_type override;

    auto clone() const -> std::unique_ptr< base > override;

private:
    using position = topology::point_type;
    using position_map = std::unordered_map< vertex_type, position >;

    position_map m_map;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph >
inline random_layout< Graph >::random_layout(
    const graph_type& g,
    const topology& space)
{
    boost::random_graph_layout(g, boost::make_assoc_property_map(m_map), space);

    assert(std::all_of(
        boost::vertices(g).first,
        boost::vertices(g).second,
        [this](auto v) { return m_map.contains(v); }));
}

template < typename Graph >
inline auto random_layout< Graph >::x(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[0];
}

template < typename Graph >
inline auto random_layout< Graph >::y(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[1];
}

template < typename Graph >
inline auto random_layout< Graph >::z(vertex_type v) const -> coord_type
{
    assert(m_map.contains(v));
    return m_map.at(v)[2];
}

template < typename Graph >
inline auto random_layout< Graph >::clone() const -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace layout

#endif // LAYOUT_RANDOM_LAYOUT_HPP
