#include "clustering/backend.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace clustering
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;
using to_id = clustering::backend_config::id_type;

TEST(
    when_making_a_clustering_backend,
    given_unplugged_clusters_at_cfg_then_unknown_plugin_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers = { "Bob" },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id),
                .k = 1 }),
        clustering::unknown_plugin);
}

TEST(
    when_making_a_clustering_backend,
    given_unplugged_mst_finders_at_cfg_then_unknown_plugin_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { "Bob" },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id),
                .k = 1 }),
        clustering::unknown_plugin);
}

TEST(
    when_making_a_clustering_backend,
    given_unlisted_cluster_at_cfg_then_unknown_default_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = "Bob",
                .mst_finder = to_id(clustering::prim_mst_id),
                .k = 1 }),
        clustering::unlisted_default);
}

TEST(
    when_making_a_clustering_backend,
    given_unlisted_mst_finder_at_cfg_then_unknown_default_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = "Bob",
                .k = 1 }),
        clustering::unlisted_default);
}

TEST(
    when_making_a_clustering_backend,
    given_k_less_than_1_at_cfg_then_invalid_k_error_is_thrown)
{
    EXPECT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers = all_clusterers(),
                .mst_finders = all_mst_finders(),
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id),
                .k = 0 }),
        clustering::invalid_k);

    EXPECT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers = all_clusterers(),
                .mst_finders = all_mst_finders(),
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id),
                .k = -1 }),
        clustering::invalid_k);
}

using backend_t = clustering::backend< graph, weight_map >;

using mock_clusterer_slot_t = NiceMock<
    MockFunction< void(const typename backend_t::clusterer_type&) > >;

using mock_mst_finder_slot_t = NiceMock<
    MockFunction< void(const typename backend_t::mst_finder_type&) > >;

using mock_k_slot_t
    = NiceMock< MockFunction< void(const typename backend_t::k_type&) > >;

class given_a_clustering_backend : public Test
{
protected:
    static constexpr auto defualt_clusterer
        = clustering::k_spanning_tree_clusterer_id;

    static constexpr auto defualt_mst_finder = clustering::prim_mst_id;

    static constexpr auto default_k = 1;

    static constexpr auto weight = 10;

    clustering::backend_config cfg { .clusterers = all_clusterers(),
                                     .mst_finders = all_mst_finders(),
                                     .clusterer = to_id(defualt_clusterer),
                                     .mst_finder = to_id(defualt_mst_finder),
                                     .k = default_k };

    graph g;
    std::unique_ptr< backend_t > backend;
    mock_clusterer_slot_t clusterer_slot;
    mock_mst_finder_slot_t mst_finder_slot;
    mock_k_slot_t k_slot;

    void SetUp() override
    {
        backend = std::make_unique< backend_t >(g, weight_map(weight), cfg);
    }
};

TEST_F(given_a_clustering_backend, given_graph_is_held)
{
    ASSERT_TRUE(boost::isomorphism(g, backend->graph()));
}

TEST_F(given_a_clustering_backend, given_weight_map_is_held)
{
    for (auto e : boost::make_iterator_range(boost::edges(backend->graph())))
        EXPECT_EQ(boost::get(backend->edge_weight(), e), weight);
}

TEST_F(given_a_clustering_backend, initially_given_default_clusterer_is_held)
{
    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
}

TEST_F(given_a_clustering_backend, initially_given_default_mst_finder_is_held)
{
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
}

TEST_F(given_a_clustering_backend, initially_given_default_k_is_held)
{
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_clusterer_with_unlisted_id_nothing_happens)
{
    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(0);
    EXPECT_CALL(k_slot, Call(_)).Times(0);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(0);

    clustering::update_clusterer(*backend, "Bob");

    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_mst_finder_with_unlisted_id_nothing_happens)
{
    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(0);
    EXPECT_CALL(k_slot, Call(_)).Times(0);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(0);

    clustering::update_mst_finder(*backend, "Bob");

    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend, when_updating_k_with_invalid_k_nothing_happens)
{
    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(0);
    EXPECT_CALL(k_slot, Call(_)).Times(0);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(0);

    clustering::update_k(*backend, -10);

    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend,
    after_updating_the_clusterer_new_clusterer_is_held)
{
    constexpr auto id = clustering::k_spanning_tree_clusterer_id;

    clustering::update_clusterer(*backend, id);

    ASSERT_EQ(clustering::get_clusterer_id(*backend), id);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_the_clusterer_observers_are_notified)
{
    constexpr auto id = clustering::k_spanning_tree_clusterer_id;

    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(clusterer_slot, Call(_)).Times(1);

    clustering::update_clusterer(*backend, id);
}

TEST_F(given_a_clustering_backend, after_updating_the_k_new_k_is_held)
{
    constexpr auto k = 80;

    clustering::update_k(*backend, k);

    ASSERT_EQ(clustering::get_k(*backend), k);
}

TEST_F(given_a_clustering_backend, when_updating_the_k_observers_are_notified)
{
    constexpr auto k = 80;

    backend->connect_to_k(k_slot.AsStdFunction());

    EXPECT_CALL(k_slot, Call(_)).Times(1);

    clustering::update_k(*backend, k);
}

TEST_F(
    given_a_clustering_backend,
    after_updating_the_mst_finder_new_mst_finder_is_held)
{
    constexpr auto id = clustering::kruskal_mst_id;

    clustering::update_mst_finder(*backend, id);

    ASSERT_EQ(clustering::get_mst_finder_id(*backend), id);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_the_mst_finder_observers_are_notified)
{
    constexpr auto id = clustering::kruskal_mst_id;

    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(1);

    clustering::update_mst_finder(*backend, id);
}

TEST_F(
    given_a_clustering_backend,
    when_restoring_to_defaults_default_data_are_given_after_querying)
{
    constexpr auto id = clustering::kruskal_mst_id;
    constexpr auto k = 3000;

    static_assert(id != defualt_mst_finder);
    static_assert(k != default_k);

    clustering::update_k(*backend, k);
    clustering::update_mst_finder(*backend, id);

    EXPECT_EQ(clustering::get_k(*backend), k);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), id);

    clustering::restore_defaults(*backend);

    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend,
    updating_k_spanning_tree_clustering_details_use_case)
{
    using expected_t
        = clustering::k_spanning_tree_clusterer< graph, weight_map >;

    constexpr auto k = 300;
    constexpr auto id = clustering::kruskal_mst_id;

    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(1);
    EXPECT_CALL(k_slot, Call(_)).Times(1);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(1);

    clustering::update_k(*backend, k);
    clustering::update_mst_finder(*backend, id);
    clustering::update_clusterer(
        *backend, clustering::k_spanning_tree_clusterer_id);

    EXPECT_EQ(clustering::get_k(*backend), k);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), id);
    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::k_spanning_tree_clusterer_id);
    ASSERT_EQ(typeid(backend->get_clusterer()), typeid(expected_t));

    const auto& downcasted
        = static_cast< const expected_t& >(backend->get_clusterer());

    EXPECT_EQ(downcasted.k(), k);
    EXPECT_EQ(downcasted.mst_finder().id(), id);
}

} // namespace clustering