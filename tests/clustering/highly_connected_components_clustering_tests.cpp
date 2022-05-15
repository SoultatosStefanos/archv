#include "graph_visualizer/clustering/highly_connected_components_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>
#include <map>

// NOTE: Ignore intelli

namespace GV::Clustering::Tests {

using namespace GV::Utils;

class Highly_connected_components_tests : public testing::Test {
protected:
    struct Vertex {
        int label{0};
    };

    struct Edge {
        int weight{0};
    };

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;
    using ParityStorage = std::map<boost::graph_traits<Graph>::vertex_descriptor, bool>;
    using ParityMap = boost::associative_property_map<ParityStorage>;
    using WeightStorage
        = std::map<boost::graph_traits<Graph>::edge_descriptor, decltype(Edge::weight)>;
    using WeightMap = boost::associative_property_map<WeightStorage>;
};

TEST_F(Highly_connected_components_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected;

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};
    highly_connected_components_clustering(initial, parity_map, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Highly_connected_components_tests, Clustering_given_min_cut)
{
    Graph g;

    auto v0 = boost::add_vertex({0}, g);
    auto v1 = boost::add_vertex({1}, g);
    auto v2 = boost::add_vertex({2}, g);
    auto v3 = boost::add_vertex({3}, g);
    auto v4 = boost::add_vertex({4}, g);
    auto v5 = boost::add_vertex({5}, g);
    auto v6 = boost::add_vertex({6}, g);
    auto v7 = boost::add_vertex({7}, g);
    auto v8 = boost::add_vertex({8}, g);

    for (auto [from, to]: {std::pair{v0, v1},
                           {v0, v2},
                           {v0, v3},
                           {v1, v3},
                           {v1, v2},
                           {v2, v3},
                           {v2, v4},
                           {v3, v5},
                           {v4, v5},
                           {v4, v6},
                           {v4, v7},
                           {v4, v8},
                           {v5, v6},
                           {v5, v7},
                           {v5, v8},
                           {v6, v7},
                           {v6, v8},
                           {v7, v8}})
        boost::add_edge(from, to, g);

    Graph expected;

    for (auto [from, to]: {std::pair{v0, v1},
                           {v0, v2},
                           {v0, v3},
                           {v1, v3},
                           {v1, v2},
                           {v2, v3},
                           {v4, v5},
                           {v4, v6},
                           {v4, v7},
                           {v4, v8},
                           {v5, v6},
                           {v5, v7},
                           {v5, v8},
                           {v6, v7},
                           {v6, v8},
                           {v7, v8}})
        boost::add_edge(from, to, expected);

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};
    highly_connected_components_clustering(g, parity_map, [](const auto& g, auto parity) {
        using VertexIter = boost::graph_traits<Graph>::vertex_iterator;
        using VertexIters = std::vector<VertexIter>;

        if (boost::num_vertices(g) != 9) return; // in recursion, nothing to set for hardcoded

        VertexIters iters;
        iters.reserve(boost::num_vertices(g));
        assert(boost::num_vertices(g) == 9);

        auto [first, last] = boost::vertices(g);
        for (auto iter = first; iter != last; ++iter) iters.push_back(iter);
        assert(iters.size() == 9);

        boost::put(parity, *iters[0], true);
        boost::put(parity, *iters[1], true);
        boost::put(parity, *iters[2], true);
        boost::put(parity, *iters[3], true);
        boost::put(parity, *iters[4], false);
        boost::put(parity, *iters[5], false);
        boost::put(parity, *iters[6], false);
        boost::put(parity, *iters[7], false);
        boost::put(parity, *iters[8], false);
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

// see https://www.boost.org/doc/libs/1_56_0/libs/graph/doc/stoer_wagner_min_cut.html
TEST_F(Highly_connected_components_tests, Clustering_with_stoer_wagner)
{
    Graph g;

    auto v0 = boost::add_vertex({0}, g);
    auto v1 = boost::add_vertex({1}, g);
    auto v2 = boost::add_vertex({2}, g);
    auto v3 = boost::add_vertex({3}, g);
    auto v4 = boost::add_vertex({4}, g);
    auto v5 = boost::add_vertex({5}, g);
    auto v6 = boost::add_vertex({6}, g);
    auto v7 = boost::add_vertex({7}, g);

    boost::add_edge(v0, v1, {2}, g);
    boost::add_edge(v0, v4, {3}, g);
    boost::add_edge(v1, v2, {3}, g);
    boost::add_edge(v1, v4, {2}, g);
    boost::add_edge(v1, v5, {2}, g);
    boost::add_edge(v5, v4, {3}, g);
    boost::add_edge(v5, v6, {1}, g);
    boost::add_edge(v2, v3, {4}, g);
    boost::add_edge(v2, v6, {2}, g);
    boost::add_edge(v3, v6, {2}, g);
    boost::add_edge(v3, v7, {2}, g);
    boost::add_edge(v6, v7, {3}, g);

    Graph expected;

    auto vv0 = boost::add_vertex({0}, expected);
    auto vv1 = boost::add_vertex({1}, expected);
    auto vv2 = boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    auto vv4 = boost::add_vertex({4}, expected);
    auto vv5 = boost::add_vertex({5}, expected);
    auto vv6 = boost::add_vertex({6}, expected);
    auto vv7 = boost::add_vertex({7}, expected);

    boost::add_edge(vv0, vv1, {2}, expected);
    boost::add_edge(vv0, vv4, {3}, expected);
    boost::add_edge(vv1, vv4, {2}, expected);
    boost::add_edge(vv1, vv5, {2}, expected);
    boost::add_edge(vv5, vv4, {3}, expected);
    boost::add_edge(vv2, vv3, {4}, expected);
    boost::add_edge(vv2, vv6, {2}, expected);
    boost::add_edge(vv3, vv6, {2}, expected);
    boost::add_edge(vv3, vv7, {2}, expected);
    boost::add_edge(vv6, vv7, {3}, expected);

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};

    WeightStorage weight_storage;
    WeightMap weight_map{weight_storage};

    const auto& [begin, end] = boost::edges(g);
    for (auto iter = begin; iter != end; ++iter) weight_storage[*iter] = g[*iter].weight;

    highly_connected_components_clustering(g, parity_map, [weight_map](const auto& g, auto parity) {
        if (boost::num_vertices(g) < 8) return; // suppose we find a min cut only once (easily test)
        boost::stoer_wagner_min_cut(g, weight_map, boost::parity_map(parity)); // ignore res
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

} // namespace GV::Clustering::Tests