// Contains a private module for the untangle_layout function implementation.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DETAIL_UNTANGLE_LAYOUT_HPP
#define LAYOUT_DETAIL_UNTANGLE_LAYOUT_HPP

#include "layout/layout.hpp" // for layout

#include <array>                          // for array
#include <boost/graph/adjacency_list.hpp> // for vertices, edges, etc
#include <boost/graph/graph_traits.hpp>   // for graph_traits
#include <cassert>                        // for assert
#include <memory>                         // for unique_ptr
#include <set>                            // for set
#include <type_traits>   // for is_convertible_v, is_default_constructible_v
#include <unordered_map> // for unordered_map

namespace layout::detail
{

template < typename Graph, typename ClusterMap >
auto make_induced_graph(const Graph& g, ClusterMap vertex_cluster) -> Graph
{
    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type [[maybe_unused]] =
        typename graph_traits::vertex_descriptor;
    using property_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename property_traits::value_type;
    using set_type = std::set< cluster_type >;

    static_assert(std::is_convertible_v< cluster_type, vertex_type >);
    static_assert(std::is_convertible_v< vertex_type, cluster_type >);

    set_type clusters_set;
    for (auto v : boost::make_iterator_range(boost::vertices(g)))
        clusters_set.insert(boost::get(vertex_cluster, v));

    Graph induced;
    for (auto c [[maybe_unused]] : clusters_set)
    {
        [[maybe_unused]] const auto v = boost::add_vertex(induced);
        assert(static_cast< vertex_type >(c) == v && "1-1 correspondence");
    }

    // Really only adding these for the layout algorithm later.
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        const auto source_cluster
            = boost::get(vertex_cluster, boost::source(e, g));
        const auto target_cluster
            = boost::get(vertex_cluster, boost::target(e, g));

        if (source_cluster != target_cluster)
            boost::add_edge(source_cluster, target_cluster, induced);
    }

    assert(boost::num_vertices(induced) == clusters_set.size());
    return induced;
}

// Just because, can't go wrong here...
template < typename Graph >
class mutable_layout : public layout< Graph >
{
    using base = layout< Graph >;
    using self = mutable_layout< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;

    mutable_layout() = default;
    ~mutable_layout() override = default;

    auto id() const -> id_type override
    {
        assert(false && "don't treat this as a plugged in type");
        return "";
    }

    auto x(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[0];
    }

    auto x(vertex_type v) -> coord_type& { return m_map[v][0]; }

    auto y(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[1];
    }

    auto y(vertex_type v) -> coord_type& { return m_map[v][1]; }

    auto z(vertex_type v) const -> coord_type override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[2];
    }

    auto z(vertex_type v) -> coord_type& { return m_map[v][2]; }

    auto clone() const -> std::unique_ptr< base > override
    {
        assert(false && "don't treat this as a plugged in type");
        return nullptr;
    }

private:
    using point_type = std::array< coord_type, 3 >;
    using storage = std::unordered_map< vertex_type, point_type >;

    storage m_map;
};

template < typename Graph, typename ClusterMap >
auto make_offsetted_layout(
    const Graph& g,
    ClusterMap vertex_cluster,
    const layout< Graph >& induced,
    const layout< Graph >& initial) -> std::unique_ptr< layout< Graph > >
{
    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type [[maybe_unused]] =
        typename graph_traits::vertex_descriptor;
    using property_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename property_traits::value_type;
    using derived_type = mutable_layout< Graph >;

    static_assert(std::is_convertible_v< cluster_type, vertex_type >);
    static_assert(std::is_convertible_v< vertex_type, cluster_type >);
    static_assert(std::is_default_constructible_v< derived_type >);

    auto res = std::make_unique< derived_type >();
    for (auto v : boost::make_iterator_range(boost::vertices(g)))
    {
        const auto c = boost::get(vertex_cluster, v);
        res->x(v) = initial.x(v) + induced.x(c);
        res->y(v) = initial.y(v) + induced.y(c);
        res->z(v) = initial.z(v) + induced.z(c);
    }

    return res;
}

} // namespace layout::detail

#endif // LAYOUT_DETAIL_UNTANGLE_LAYOUT_HPP
