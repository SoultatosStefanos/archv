// Contains the clustering interface of the application.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTERER_HPP
#define CLUSTERING_CLUSTERER_HPP

#include <boost/graph/graph_concepts.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace clustering
{

/***********************************************************
 * Clustering Interface                                    *
 ***********************************************************/

// Returns a cluster map, where each graph vertex is assigned an integral c.
// For a graph g: c e { 0, ..., num_vertices(g) }
template < typename Graph >
class clusterer
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    using self = clusterer< Graph >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = std::string_view;
    using graph_type = Graph;
    using vertex_type = typename graph_traits ::vertex_descriptor;

    using cluster = unsigned long long;
    using cluster_map = std::unordered_map< vertex_type, cluster >;

    clusterer() = default;
    clusterer(const clusterer&) = default;
    clusterer(clusterer&&) = default;
    virtual ~clusterer() = default;

    auto operator=(const clusterer&) -> clusterer& = default;
    auto operator=(clusterer&&) -> clusterer& = default;

    virtual auto id() const -> id_type = 0;

    virtual auto operator()(const graph_type& g) const -> cluster_map = 0;

    virtual auto clone() const -> std::unique_ptr< self > = 0;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// Alternative interface to invoking a clusterer.
template < typename Graph >
inline auto cluster(const Graph& g, const clusterer< Graph >& clusterer)
{
    return clusterer(g);
}

} // namespace clustering

#endif // CLUSTERING_CLUSTERER_HPP
