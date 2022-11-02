#include "clustering/backend.hpp"
#include "clustering/cluster_map.hpp"
#include "misc/random.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using backend_t = clustering::backend< graph, weight_map >;

inline auto make_graph()
{
    graph g;
    const auto nedges = misc::urandom(3, 10);

    for (auto i = 0; i < nedges; ++i)
        boost::add_edge(boost::add_vertex(g), boost::add_vertex(g), g);

    return g;
}

class a_clustering_property_map : public Test
{
protected:
    clustering::backend_config cfg = clustering::default_backend_config();

    graph g = make_graph();
    std::unique_ptr< backend_t > backend;

    void SetUp() override
    {
        backend = std::make_unique< backend_t >(g, weight_map(10), cfg);
    }
};

TEST_F(a_clustering_property_map, dispatches_clusters_from_clustering_backend)
{
    clustering::update_clusters(*backend);

    auto cluster_map = clustering::make_cluster_map(*backend);

    for (auto v : boost::make_iterator_range(boost::vertices(g)))
        ASSERT_EQ(boost::get(cluster_map, v), backend->get_clusters().at(v));
}

} // namespace
