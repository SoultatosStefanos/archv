// Contains a graph_type 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include <boost/graph/graph_concepts.hpp>
#include <memory>

namespace layout
{

template < typename Graph >
class layout_visitor;

// In 3D space.
template < typename Graph >
class layout
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

public:
    using graph_type = Graph;
    using vertex_type = typename graph_type::vertex_descriptor;
    using coord_type = double;
    using visitor_type = layout_visitor< graph_type >;

    layout() = default;
    layout(const layout&) = default;
    layout(layout&&) = default;
    virtual ~layout() = default;

    auto operator=(const layout&) -> layout& = default;
    auto operator=(layout&&) -> layout& = default;

    virtual auto x(vertex_type v) const -> coord_type = 0;
    virtual auto y(vertex_type v) const -> coord_type = 0;
    virtual auto z(vertex_type v) const -> coord_type = 0;

    virtual auto accept(const visitor_type& visitor) const -> void = 0;

    virtual auto clone() const -> std::unique_ptr< layout< graph_type > > = 0;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_HPP
