// Contains a 3D layout class implementing the gursoy atun algorithm.
// Soultatos Stefanos 2022

#ifndef LAYOUT_GURSOY_ATUN_LAYOUT_HPP
#define LAYOUT_GURSOY_ATUN_LAYOUT_HPP

#include "detail/gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <cassert>

namespace layout
{

// Assigns a position, at a 3d space, to each graph_type vertex_type.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph_type/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.

template < typename Graph >
class gursoy_atun_layout : public layout< Graph >
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

public:
    using base = layout< Graph >;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;

    template < typename WeightMap >
    gursoy_atun_layout(
        const graph_type& g, const topology& space, WeightMap edge_weight)
    {
        BOOST_CONCEPT_ASSERT(
            (boost::ReadablePropertyMapConcept<
                WeightMap,
                typename boost::graph_traits< graph_type >::edge_descriptor >));

        space.accept(detail::gursoy_atun_visitor(
            g, edge_weight, boost::make_assoc_property_map(m_map)));

        assert(std::all_of(
            boost::vertices(g).first,
            boost::vertices(g).second,
            [this](auto v) { return m_map.contains(v); }));
    }

    virtual ~gursoy_atun_layout() override = default;

    virtual auto x(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[0];
    }

    virtual auto y(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[1];
    }

    virtual auto z(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[2];
    }

    virtual auto accept(const layout_visitor< graph_type >& visitor) const
        -> void override
    {
        visitor.visit(*this);
    }

    virtual auto clone() const
        -> std::unique_ptr< layout< graph_type > > override
    {
        return std::make_unique< gursoy_atun_layout< graph_type > >(*this);
    }

private:
    using position_map
        = std::unordered_map< vertex_type, topology::point_type >;

    position_map m_map;
};

} // namespace layout

#endif // LAYOUT_GURSOY_ATUN_LAYOUT_HPP
