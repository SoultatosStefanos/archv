#include "rendering/degrees_ranked_backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace rendering;

namespace
{

inline auto initial_data()
{
    return degrees_ranked_config {
        .in_data = degree_evaluation_data(
            make_ranked(1, 3, 5),
            make_ranked< std::string >("a", "b", "c"),
            true),
        .out_data = degree_evaluation_data(
            make_ranked(2, 4, 6),
            make_ranked< std::string >("aa", "bb", "cc"),
            true)
    };
}

class given_a_degrees_degrees_backend : public Test
{
protected:
    using mock_type
        = NiceMock< MockFunction< void(const degree_evaluation_data&) > >;

    void SetUp() override
    {
        b = std::make_unique< degrees_ranked_backend >(initial_data());
    }

    std::unique_ptr< degrees_ranked_backend > b;
    mock_type mock;
};

TEST_F(
    given_a_degrees_degrees_backend,
    in_degree_evaluation_is_applied_on_in_data_evaluation_data)
{
    EXPECT_EQ(evaluate_in_degree(*b, 0), null_particles);
    EXPECT_EQ(evaluate_in_degree(*b, 1), "a");
    EXPECT_EQ(evaluate_in_degree(*b, 2), "a");
    EXPECT_EQ(evaluate_in_degree(*b, 3), "b");
    EXPECT_EQ(evaluate_in_degree(*b, 4), "b");
    EXPECT_EQ(evaluate_in_degree(*b, 5), "c");
    EXPECT_EQ(evaluate_in_degree(*b, 100), "c");
}

TEST_F(
    given_a_degrees_degrees_backend,
    out_degree_evaluation_is_applied_on_out_data_evaluation_data)
{
    EXPECT_EQ(evaluate_out_degree(*b, 0), null_particles);
    EXPECT_EQ(evaluate_out_degree(*b, 2), "aa");
    EXPECT_EQ(evaluate_out_degree(*b, 3), "aa");
    EXPECT_EQ(evaluate_out_degree(*b, 4), "bb");
    EXPECT_EQ(evaluate_out_degree(*b, 5), "bb");
    EXPECT_EQ(evaluate_out_degree(*b, 6), "cc");
    EXPECT_EQ(evaluate_out_degree(*b, 100), "cc");
}

TEST_F(
    given_a_degrees_degrees_backend,
    updates_with_negative_light_threshold_are_ignored)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_)).Times(0);

    update_in_degree_evaluation_light_threshold(*b, -1);
    update_out_degree_evaluation_light_threshold(*b, -1);
}

TEST_F(
    given_a_degrees_degrees_backend,
    updates_with_medium_threshold_which_is_not_greater_to_the_light_are_ignored)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_)).Times(0);

    update_out_degree_evaluation_medium_threshold(*b, 1);
    update_out_degree_evaluation_medium_threshold(*b, 2);
}

TEST_F(
    given_a_degrees_degrees_backend,
    updates_with_heavy_threshold_which_is_not_greater_to_the_medium_are_ignored)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_)).Times(0);

    update_out_degree_evaluation_heavy_threshold(*b, 3);
    update_out_degree_evaluation_heavy_threshold(*b, 4);
}

TEST_F(
    given_a_degrees_degrees_backend, its_config_data_are_equal_to_the_initial)
{
    ASSERT_EQ(b->config_data(), initial_data());
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_evaluation_data_observers_are_notified)
{
    auto new_data = degree_evaluation_data(
        make_ranked(1, 2, 3), make_ranked< std::string >("", "", ""), true);
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation(*b, new_data);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_evaluation_data_access_to_them_yields_new_data)
{
    auto new_data = degree_evaluation_data(
        make_ranked(1, 2, 3), make_ranked< std::string >("", "", ""), true);

    update_in_degree_evaluation(*b, new_data);

    ASSERT_EQ(get_in_degree_evaluation_data(*b), new_data);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_evaluation_data_observers_are_notified)
{
    auto new_data = degree_evaluation_data(
        make_ranked(1, 2, 3), make_ranked< std::string >("", "", ""), true);
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation(*b, new_data);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_evaluation_data_access_to_them_yields_new_data)
{
    auto new_data = degree_evaluation_data(
        make_ranked(1, 2, 3), make_ranked< std::string >("", "", ""), true);

    update_out_degree_evaluation(*b, new_data);

    ASSERT_EQ(get_out_degree_evaluation_data(*b), new_data);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_light_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 2;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(new_threshold, 3, 5),
                                            { "a", "b", "c" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_light_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_light_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 2;

    update_in_degree_evaluation_light_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_light_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_light_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(3, 4, 6),
                                            { "aa", "bb", "cc" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_light_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_light_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_out_degree_evaluation_light_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_light_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_medium_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "b", "c" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_medium_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_medium_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_in_degree_evaluation_medium_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_medium_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_medium_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 3, 6),
                                            { "aa", "bb", "cc" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_medium_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_medium_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_out_degree_evaluation_medium_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_medium_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_heavy_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 100;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, new_threshold),
                                            { "a", "b", "c" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_heavy_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_heavy_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 100;

    update_in_degree_evaluation_heavy_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_heavy_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_heavy_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 200;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 4, new_threshold),
                                            { "aa", "bb", "cc" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_heavy_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_heavy_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 100;

    update_out_degree_evaluation_heavy_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_heavy_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_light_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "bob", "b", "c" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_light_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_light_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_light_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_light_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_light_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "bob", "bb", "cc" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_light_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_light_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_light_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_light_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_medium_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "bob", "c" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_medium_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_medium_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_medium_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_medium_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_medium_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "aa", "bob", "cc" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_medium_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_medium_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_medium_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_medium_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_heavy_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "b", "bob" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_heavy_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_heavy_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_heavy_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_heavy_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_heavy_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "aa", "bb", "bob" },
                                            true };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_heavy_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_heavy_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_heavy_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_heavy_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_applied_prop_observers_are_notified)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "b", "c" },
                                            false };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_applied(*b, false);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_in_degree_applied_prop_access_it_yields_new_effect)
{
    update_in_degree_evaluation_applied(*b, false);

    ASSERT_FALSE(is_in_degree_evaluation_applied(*b));
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_applied_prop_observers_are_notified)
{
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const degree_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "aa", "bb", "cc" },
                                            false };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_applied(*b, false);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_updating_the_out_degree_applied_prop_access_it_yields_new_effect)
{
    update_out_degree_evaluation_applied(*b, false);

    ASSERT_FALSE(is_out_degree_evaluation_applied(*b));
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_restoring_the_in_degree_evaluation_observers_are_notified)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(initial_data().in_data)).Times(1);

    restore_in_degree_evaluation(*b);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_restoring_the_in_degree_evaluation_access_to_it_yield_original)
{
    update_in_degree_evaluation(*b, initial_data().out_data);
    restore_in_degree_evaluation(*b);

    ASSERT_EQ(get_in_degree_evaluation_data(*b), initial_data().in_data);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_restoring_the_out_degree_evaluation_observers_are_notified)
{
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(initial_data().out_data)).Times(1);

    restore_out_degree_evaluation(*b);
}

TEST_F(
    given_a_degrees_degrees_backend,
    after_restoring_the_out_degree_evaluation_access_to_it_yield_original)
{
    update_out_degree_evaluation(*b, initial_data().in_data);
    restore_out_degree_evaluation(*b);

    ASSERT_EQ(get_out_degree_evaluation_data(*b), initial_data().out_data);
}

} // namespace
