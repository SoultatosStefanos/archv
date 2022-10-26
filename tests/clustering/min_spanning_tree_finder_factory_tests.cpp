#include "clustering/min_spanning_tree_finder_factory.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

using mst_finder_factory
    = clustering::min_spanning_tree_finder_factory< graph, weight_map >;

TEST(clusterin_mst_finder_factory_tests, given_prim_id_returns_prim_mst)
{
    constexpr auto id = clustering::prim_mst_id;
    static_assert(clustering::is_mst_finder_plugged_in(id));
    auto finder = mst_finder_factory::make_mst_finder(id);

    ASSERT_NE(nullptr, finder);
    ASSERT_EQ(id, finder->id());
}

TEST(clusterin_mst_finder_factory_tests, given_kruskal_id_returns_kruskal_mst)
{
    constexpr auto id = clustering::kruskal_mst_id;
    static_assert(clustering::is_mst_finder_plugged_in(id));
    auto finder = mst_finder_factory::make_mst_finder(id);

    ASSERT_NE(nullptr, finder);
    ASSERT_EQ(id, finder->id());
}

} // namespace