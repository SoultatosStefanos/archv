#include "clustering/strong_components_clustering.hpp"
#include "utility.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>

namespace
{

using namespace Clustering;
using namespace Utility;

class Highly_connected_components_tests : public testing::Test
{
protected:
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
};

TEST_F(Highly_connected_components_tests, Empty_yields_empty)
{
    Graph g, expected;

    std::vector<int> c(boost::num_vertices(g));
    strong_components_clustering(
        g, boost::make_iterator_property_map(c.begin(), boost::get(boost::vertex_index, g)));

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

// see https://www.boost.org/doc/libs/1_65_0/libs/graph/example/strong_components.cpp
TEST_F(Highly_connected_components_tests, Clustering)
{
    Graph g;

    for (auto i = 0; i < 10; ++i)
        boost::add_vertex(g);

    boost::add_edge(0, 1, g);
    boost::add_edge(0, 5, g);
    boost::add_edge(0, 7, g);
    boost::add_edge(1, 2, g);
    boost::add_edge(1, 0, g);
    boost::add_edge(2, 3, g);
    boost::add_edge(2, 1, g);
    boost::add_edge(3, 4, g);
    boost::add_edge(4, 3, g);
    boost::add_edge(5, 6, g);
    boost::add_edge(6, 5, g);
    boost::add_edge(6, 3, g);
    boost::add_edge(7, 8, g);
    boost::add_edge(8, 7, g);
    boost::add_edge(8, 9, g);
    boost::add_edge(8, 4, g);
    boost::add_edge(8, 2, g);

    Graph expected;

    for (auto i = 0; i < 10; ++i)
        boost::add_vertex(expected);

    boost::add_edge(0, 1, expected);
    boost::add_edge(0, 7, expected);
    boost::add_edge(1, 2, expected);
    boost::add_edge(1, 0, expected);
    boost::add_edge(2, 1, expected);
    boost::add_edge(3, 4, expected);
    boost::add_edge(4, 3, expected);
    boost::add_edge(5, 6, expected);
    boost::add_edge(6, 5, expected);
    boost::add_edge(7, 8, expected);
    boost::add_edge(8, 7, expected);
    boost::add_edge(8, 2, expected);

    std::vector<int> c(boost::num_vertices(g));
    strong_components_clustering(
        g, boost::make_iterator_property_map(c.begin(), boost::get(boost::vertex_index, g)));

    ASSERT_TRUE(boost::isomorphism(expected, g));
}

} // namespace