// Contains a graph_type 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include <boost/graph/graph_concepts.hpp> // for GraphConcept
#include <memory>                         // for unique_ptr
#include <string_view>                    // for string_view

namespace layout
{

// In 3D space.
template < typename Graph >
class layout
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    using self = layout< Graph >;

public:
    using id_type = std::string_view;
    using graph_type = Graph;
    using vertex_type = typename graph_type::vertex_descriptor;
    using coord_type = double;

    layout() = default;
    layout(const layout&) = default;
    layout(layout&&) = default;
    virtual ~layout() = default;

    auto operator=(const layout&) -> layout& = default;
    auto operator=(layout&&) -> layout& = default;

    virtual auto id() const -> id_type = 0;

    virtual auto x(vertex_type v) const -> coord_type = 0;
    virtual auto y(vertex_type v) const -> coord_type = 0;
    virtual auto z(vertex_type v) const -> coord_type = 0;

    virtual auto clone() const -> std::unique_ptr< self > = 0;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_HPP
