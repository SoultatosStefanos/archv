// Contains a graph 3D layout interface.
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
    using graph = Graph;
    using vertex = typename boost::graph_traits< graph >::vertex_descriptor;
    using coord = double;
    using descriptor = std::string;

    layout() = default;
    layout(const layout&) = default;
    layout(layout&&) = default;
    virtual ~layout() = default;

    auto operator=(const layout&) -> layout& = default;
    auto operator=(layout&&) -> layout& = default;

    // For runtime type identification.
    virtual auto desc() const -> descriptor = 0;

    virtual auto x(vertex v) const -> coord = 0;
    virtual auto y(vertex v) const -> coord = 0;
    virtual auto z(vertex v) const -> coord = 0;

    virtual void accept(const layout_visitor< graph >& visitor) const = 0;

    virtual auto clone() const -> std::unique_ptr< layout< graph > > = 0;
};

template < typename Layout >
struct layout_traits
{
    using graph = typename Layout::graph;
    using vertex = typename Layout::vertex;
    using coord = typename Layout::coord;
    using descriptor = typename Layout::descriptor;

    static constexpr auto desc() -> auto { return Layout::description; }
};

} // namespace layout

#endif // LAYOUT_LAYOUT_HPP
