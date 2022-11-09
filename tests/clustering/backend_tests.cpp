#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

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
            clustering::backend_config { .clusterers = { "Bob" } }),
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
                .mst_finders = { "Bob" } }),
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
                .clusterer = "Bob" }),
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
                .mst_finder = "Bob" }),
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
}

TEST(
    when_making_a_clustering_backend,
    given_snn_thres_less_than_0_at_cfg_then_invalid_snn_threshold_error_is_thrown)
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
                .k = 2,
                .snn_threshold = -1 }),
        clustering::invalid_snn_threshold);
}

using backend_t = clustering::backend< graph, weight_map >;

using mock_clusters_slot_t = NiceMock<
    MockFunction< void(const typename backend_t::cluster_map_type&) > >;

using mock_clusterer_slot_t = NiceMock<
    MockFunction< void(const typename backend_t::clusterer_type&) > >;

using mock_mst_finder_slot_t = NiceMock<
    MockFunction< void(const typename backend_t::mst_finder_type&) > >;

using mock_k_slot_t
    = NiceMock< MockFunction< void(typename backend_t::k_type) > >;

using mock_snn_thres_slot_t
    = NiceMock< MockFunction< void(typename backend_t::snn_threshold_type) > >;

using mock_modularity_slot_t
    = NiceMock< MockFunction< void(typename backend_t::modularity_type) > >;

class given_a_clustering_backend : public Test
{
protected:
    static constexpr auto defualt_clusterer
        = clustering::k_spanning_tree_clusterer_id;

    static constexpr auto defualt_mst_finder = clustering::prim_mst_id;

    static constexpr auto default_k = 3;

    static constexpr auto default_snn_threshold = 3;

    static constexpr auto default_min_q = 0.5f;

    static constexpr auto weight = 10;

    clustering::backend_config cfg = clustering::default_backend_config();

    graph g;
    std::unique_ptr< backend_t > backend;
    mock_clusters_slot_t clusters_slot;
    mock_clusterer_slot_t clusterer_slot;
    mock_mst_finder_slot_t mst_finder_slot;
    mock_k_slot_t k_slot;
    mock_snn_thres_slot_t snn_thres_slot;
    mock_modularity_slot_t modularity_slot;

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
    given_a_clustering_backend, initially_given_default_snn_threshold_is_held)
{
    ASSERT_EQ(clustering::get_snn_threshold(*backend), default_snn_threshold);
}

TEST_F(given_a_clustering_backend, initially_given_min_modularity_is_held)
{
    ASSERT_EQ(clustering::get_min_modularity(*backend), default_min_q);
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
    when_updating_snn_threshold_with_invalid_value_nothing_happens)
{
    backend->connect_to_snn_threshold(snn_thres_slot.AsStdFunction());
    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(snn_thres_slot, Call(_)).Times(0);
    EXPECT_CALL(mst_finder_slot, Call(_)).Times(0);
    EXPECT_CALL(k_slot, Call(_)).Times(0);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(0);

    clustering::update_snn_threshold(*backend, -1);

    ASSERT_EQ(clustering::get_snn_threshold(*backend), default_snn_threshold);
    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_the_clusters_observers_are_notified)
{
    backend->connect_to_clusters(clusters_slot.AsStdFunction());

    EXPECT_CALL(clusters_slot, Call(_)).Times(1);

    clustering::update_clusters(*backend);
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
    after_updating_the_snn_threshold_new_val_is_held)
{
    constexpr auto t = 80;

    clustering::update_snn_threshold(*backend, t);

    ASSERT_EQ(clustering::get_snn_threshold(*backend), t);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_the_snn_threshold_observers_are_notified)
{
    constexpr auto t = 80;

    backend->connect_to_snn_threshold(snn_thres_slot.AsStdFunction());

    EXPECT_CALL(snn_thres_slot, Call(t)).Times(1);

    clustering::update_snn_threshold(*backend, t);
}

TEST_F(
    given_a_clustering_backend,
    after_updating_the_min_modularity_new_val_is_held)
{
    constexpr auto q = 0.5f;

    clustering::update_min_modularity(*backend, q);

    ASSERT_EQ(clustering::get_min_modularity(*backend), q);
}

TEST_F(
    given_a_clustering_backend,
    when_updating_the_min_modularity_observers_are_notified)
{
    constexpr auto q = 0.5f;

    backend->connect_to_min_modularity(modularity_slot.AsStdFunction());

    EXPECT_CALL(modularity_slot, Call(q)).Times(1);

    clustering::update_min_modularity(*backend, q);
}

TEST_F(
    given_a_clustering_backend,
    when_restoring_to_defaults_default_data_are_given_after_querying)
{
    constexpr auto id = clustering::kruskal_mst_id;
    constexpr auto k = 3000;
    constexpr auto snn_thres = 3434;

    static_assert(id != defualt_mst_finder);
    static_assert(k != default_k);
    static_assert(snn_thres != default_snn_threshold);

    clustering::update_k(*backend, k);
    clustering::update_mst_finder(*backend, id);
    clustering::update_snn_threshold(*backend, snn_thres);

    EXPECT_EQ(clustering::get_k(*backend), k);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), id);
    EXPECT_EQ(clustering::get_snn_threshold(*backend), snn_thres);

    clustering::restore_defaults(*backend);

    ASSERT_EQ(clustering::get_clusterer_id(*backend), defualt_clusterer);
    ASSERT_EQ(clustering::get_mst_finder_id(*backend), defualt_mst_finder);
    ASSERT_EQ(clustering::get_k(*backend), default_k);
    ASSERT_EQ(clustering::get_snn_threshold(*backend), default_snn_threshold);
}

TEST_F(
    given_a_clustering_backend,
    updating_k_spanning_tree_clustering_details_use_case)
{
    using expected_t
        = clustering::k_spanning_tree_clusterer< graph, weight_map >;

    constexpr auto k = 300;
    constexpr auto id = clustering::k_spanning_tree_clusterer_id;
    constexpr auto mst_id = clustering::kruskal_mst_id;

    backend->connect_to_mst_finder(mst_finder_slot.AsStdFunction());
    backend->connect_to_k(k_slot.AsStdFunction());
    backend->connect_to_clusterer(clusterer_slot.AsStdFunction());

    EXPECT_CALL(mst_finder_slot, Call(_)).Times(1);
    EXPECT_CALL(k_slot, Call(_)).Times(1);
    EXPECT_CALL(clusterer_slot, Call(_)).Times(1);

    clustering::update_k(*backend, k);
    clustering::update_mst_finder(*backend, mst_id);
    clustering::update_clusterer(*backend, id);

    EXPECT_EQ(clustering::get_k(*backend), k);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), mst_id);
    EXPECT_EQ(clustering::get_clusterer_id(*backend), id);
    ASSERT_EQ(typeid(backend->get_clusterer()), typeid(expected_t));

    const auto& downcasted
        = static_cast< const expected_t& >(backend->get_clusterer());

    EXPECT_EQ(downcasted.k(), k);
    EXPECT_EQ(downcasted.mst_finder().id(), mst_id);
}

TEST_F(given_a_clustering_backend, updating_snn_clustering_details_use_case)
{
    using expected_t = clustering::shared_nearest_neighbour_clusterer< graph >;

    constexpr auto thres = 300;
    constexpr auto id = clustering::snn_clusterer_id;

    backend->connect_to_snn_threshold(snn_thres_slot.AsStdFunction());

    EXPECT_CALL(snn_thres_slot, Call(_)).Times(1);

    clustering::update_snn_threshold(*backend, thres);
    clustering::update_clusterer(*backend, id);

    EXPECT_EQ(clustering::get_snn_threshold(*backend), thres);
    EXPECT_EQ(clustering::get_clusterer_id(*backend), id);
    ASSERT_EQ(typeid(backend->get_clusterer()), typeid(expected_t));

    const auto& downcasted
        = static_cast< const expected_t& >(backend->get_clusterer());

    EXPECT_EQ(downcasted.threshold(), thres);
}

} // namespace clustering

#pragma GCC diagnostic pop