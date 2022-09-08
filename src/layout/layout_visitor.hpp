// Contains a 3D layout visitor interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_VISITOR_HPP
#define LAYOUT_LAYOUT_VISITOR_HPP

#include <boost/graph/graph_concepts.hpp>

namespace layout
{

template < typename Graph >
class gursoy_atun_layout;

template < typename Graph >
class layout_visitor
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

public:
    using graph_type = Graph;
    using gursoy_atun_type = gursoy_atun_layout< graph_type >;

    layout_visitor() = default;
    layout_visitor(const layout_visitor&) = default;
    layout_visitor(layout_visitor&&) = default;
    virtual ~layout_visitor() = default;

    auto operator=(const layout_visitor&) -> layout_visitor& = default;
    auto operator=(layout_visitor&&) -> layout_visitor& = default;

    virtual auto visit(const gursoy_atun_type&) const -> void = 0;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_VISITOR_HPP
