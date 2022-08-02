// Contains a 3D layout visitor interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_VISITOR_HPP
#define LAYOUT_LAYOUT_VISITOR_HPP

namespace layout
{

template <typename Graph>
class gursoy_atun_layout;

template <typename Graph>
class layout_visitor
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

public:
    using graph = Graph;

    layout_visitor() = default;
    layout_visitor(const layout_visitor&) = default;
    layout_visitor(layout_visitor&&) = default;
    virtual ~layout_visitor() = default;

    auto operator=(const layout_visitor&) -> layout_visitor& = default;
    auto operator=(layout_visitor&&) -> layout_visitor& = default;

    virtual void visit(const gursoy_atun_layout<graph>& c) const = 0;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_VISITOR_HPP
