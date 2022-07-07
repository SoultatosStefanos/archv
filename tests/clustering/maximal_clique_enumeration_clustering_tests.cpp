#include "clustering/maximal_clique_enumeration_clustering.hpp"
#include "utility/all.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/isomorphism.hpp>

namespace
{

using namespace clustering;
using namespace utility;

class Maximal_clique_enumeration_tests : public testing::Test
{
protected:
    using Graph =
        boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::no_property>;
};

TEST_F(Maximal_clique_enumeration_tests, Empty_yields_empty)
{
    Graph initial, expected;

    maximum_clique_enumeration_clustering(initial,
                                          [](const auto&, const auto&) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Maximal_clique_enumeration_tests, Custering_with_bron_kerbosh)
{
    Graph g;

    auto v4 = boost::add_vertex(4, g);
    auto v5 = boost::add_vertex(5, g);
    auto v6 = boost::add_vertex(6, g);
    auto v7 = boost::add_vertex(7, g);
    auto v8 = boost::add_vertex(8, g);

    boost::add_edge(v4, v7, g);
    boost::add_edge(v4, v8, g);
    boost::add_edge(v5, v6, g);
    boost::add_edge(v5, v7, g);
    boost::add_edge(v5, v8, g);
    boost::add_edge(v6, v8, g);
    boost::add_edge(v6, v7, g);
    boost::add_edge(v7, v8, g);

    Graph expected;

    boost::add_vertex(4, expected);
    auto vv5 = boost::add_vertex(5, expected);
    auto vv6 = boost::add_vertex(6, expected);
    auto vv7 = boost::add_vertex(7, expected);
    auto vv8 = boost::add_vertex(8, expected);

    boost::add_edge(vv5, vv6, expected);
    boost::add_edge(vv5, vv7, expected);
    boost::add_edge(vv5, vv8, expected);
    boost::add_edge(vv6, vv8, expected);
    boost::add_edge(vv6, vv7, expected);
    boost::add_edge(vv7, vv8, expected);

    maximum_clique_enumeration_clustering(
        g, [](const auto& g, const auto& visitor) {
            boost::bron_kerbosch_all_cliques(g, visitor);
        });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

} // namespace