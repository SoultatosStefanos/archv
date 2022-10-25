// Contains the clustering interface of the application.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTERER_HPP
#define CLUSTERING_CLUSTERER_HPP

#include <boost/graph/graph_concepts.hpp>
#include <memory>
#include <unordered_map>

namespace clustering
{

template < typename Graph >
class clusterer_visitor;

template < typename Graph >
class clusterer
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

public:
    using graph_type = Graph;
    using vertex_type = typename graph_type ::vertex_descriptor;
    using visitor_type = clusterer_visitor< graph_type >;

    using cluster = int;
    using cluster_map = std::unordered_map< vertex_type, cluster >;

    clusterer() = default;
    clusterer(const clusterer&) = default;
    clusterer(clusterer&&) = default;
    virtual ~clusterer() = default;

    auto operator=(const clusterer&) -> clusterer& = default;
    auto operator=(clusterer&&) -> clusterer& = default;

    virtual auto operator()() const -> cluster_map = 0;

    virtual auto accept(const visitor_type& visitor) const -> void = 0;

    virtual auto clone() const -> std::unique_ptr< clusterer< Graph > > = 0;
};

} // namespace clustering

#endif // CLUSTERING_CLUSTERER_HPP
