#include "graph_visualizer/clustering/shared_nearest_neighbour.hpp"
#include "graph_visualizer/clustering/shared_nearest_neighbour_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <map>

namespace GV::Clustering::Tests {

using namespace GV::Utils;

class Shared_nearest_neighbour_tests : public testing::Test {
protected:
    struct Vertex {
        int label{0};
    };

    struct Edge {
        int weight{0};
    };

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;
};

TEST_F(Shared_nearest_neighbour_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected = initial; // empty
    const auto threshold = urandom(1, 10);

    shared_nearest_neighbour_clustering(initial, threshold, boost::get(&Edge::weight, initial));

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Shared_nearest_neighbour_tests, Clustering_computing_snn)
{
    constexpr auto threshold = 3;

    Graph actual;

    auto v0 = boost::add_vertex({0}, actual);
    auto v1 = boost::add_vertex({1}, actual);
    auto v2 = boost::add_vertex({2}, actual);
    auto v3 = boost::add_vertex({3}, actual);
    auto v4 = boost::add_vertex({4}, actual);

    boost::add_edge(v0, v1, actual);
    boost::add_edge(v0, v2, actual);
    boost::add_edge(v0, v3, actual);
    boost::add_edge(v1, v2, actual);
    boost::add_edge(v1, v3, actual);
    boost::add_edge(v3, v2, actual);
    boost::add_edge(v3, v4, actual);
    boost::add_edge(v4, v2, actual);

    Graph expected;

    boost::add_vertex({0}, expected);
    boost::add_vertex({1}, expected);
    auto vv2 = boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    boost::add_vertex({4}, expected);

    boost::add_edge(vv3, vv2, expected);

    using WeightStorage
        = std::map<boost::graph_traits<Graph>::edge_descriptor, decltype(Edge::weight)>;
    using WeightMap = boost::associative_property_map<WeightStorage>;

    WeightStorage weight_storage;
    WeightMap weight_map{weight_storage};

    shared_nearest_neighbour_clustering(actual, threshold, weight_map);

    ASSERT_TRUE(boost::isomorphism(actual, expected));
}

} // namespace GV::Clustering::Tests