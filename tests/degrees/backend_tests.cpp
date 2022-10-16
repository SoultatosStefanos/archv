#include "degrees/backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace degrees;

namespace
{

inline auto initial_data()
{
    return std::make_pair(
        effect_evaluation_data(
            make_ranked(1, 3, 5), make_ranked< std::string >("a", "b", "c")),
        effect_evaluation_data(
            make_ranked(2, 4, 6),
            make_ranked< std::string >("aa", "bb", "cc")));
}

class given_a_degrees_backend : public Test
{
protected:
    using mock_type
        = NiceMock< MockFunction< void(const effect_evaluation_data&) > >;

    void SetUp() override { b = std::make_unique< backend >(initial_data()); }

    std::unique_ptr< backend > b;
    mock_type mock;
};

TEST_F(
    given_a_degrees_backend,
    in_degree_evaluation_is_applied_on_first_evaluation_data)
{
    EXPECT_EQ(evaluate_in_degree(*b, 0), null_effect);
    EXPECT_EQ(evaluate_in_degree(*b, 1), "a");
    EXPECT_EQ(evaluate_in_degree(*b, 2), "a");
    EXPECT_EQ(evaluate_in_degree(*b, 3), "b");
    EXPECT_EQ(evaluate_in_degree(*b, 4), "b");
    EXPECT_EQ(evaluate_in_degree(*b, 5), "c");
    EXPECT_EQ(evaluate_in_degree(*b, 100), "c");
}

TEST_F(
    given_a_degrees_backend,
    out_degree_evaluation_is_applied_on_second_evaluation_data)
{
    EXPECT_EQ(evaluate_out_degree(*b, 0), null_effect);
    EXPECT_EQ(evaluate_out_degree(*b, 2), "aa");
    EXPECT_EQ(evaluate_out_degree(*b, 3), "aa");
    EXPECT_EQ(evaluate_out_degree(*b, 4), "bb");
    EXPECT_EQ(evaluate_out_degree(*b, 5), "bb");
    EXPECT_EQ(evaluate_out_degree(*b, 6), "cc");
    EXPECT_EQ(evaluate_out_degree(*b, 100), "cc");
}

TEST_F(given_a_degrees_backend, its_config_data_are_equal_to_the_initial)
{
    ASSERT_EQ(b->config_data(), initial_data());
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_evaluation_data_observers_are_notified)
{
    auto new_data = effect_evaluation_data();
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation(*b, new_data);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_evaluation_data_access_to_them_yields_new_data)
{
    auto new_data = effect_evaluation_data();

    update_in_degree_evaluation(*b, new_data);

    ASSERT_EQ(get_in_degree_effect_evaluation_data(*b), new_data);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_evaluation_data_observers_are_notified)
{
    auto new_data = effect_evaluation_data();
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation(*b, new_data);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_evaluation_data_access_to_them_yields_new_data)
{
    auto new_data = effect_evaluation_data();

    update_out_degree_evaluation(*b, new_data);

    ASSERT_EQ(get_out_degree_effect_evaluation_data(*b), new_data);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_light_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(3, 3, 5),
                                            { "a", "b", "c" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_light_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_light_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_in_degree_evaluation_light_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_light_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_light_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(3, 4, 6),
                                            { "aa", "bb", "cc" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_light_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_light_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_out_degree_evaluation_light_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_light_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_medium_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "b", "c" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_medium_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_medium_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_in_degree_evaluation_medium_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_medium_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_medium_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(2, 3, 6),
                                            { "aa", "bb", "cc" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_medium_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_medium_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_out_degree_evaluation_medium_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_medium_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_heavy_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(1, 3, 3),
                                            { "a", "b", "c" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_heavy_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_heavy_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_in_degree_evaluation_heavy_threshold(*b, new_threshold);

    ASSERT_EQ(get_in_degree_evaluation_heavy_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_heavy_threshold_observers_are_notified)
{
    constexpr auto new_threshold = 3;
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(2, 4, 3),
                                            { "aa", "bb", "cc" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_heavy_threshold(*b, new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_heavy_threshold_access_it_yields_new_threshold)
{
    constexpr auto new_threshold = 3;

    update_out_degree_evaluation_heavy_threshold(*b, new_threshold);

    ASSERT_EQ(get_out_degree_evaluation_heavy_threshold(*b), new_threshold);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_light_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "bob", "b", "c" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_light_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_light_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_light_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_light_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_light_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "bob", "bb", "cc" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_light_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_light_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_light_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_light_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_medium_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "bob", "c" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_medium_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_medium_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_medium_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_medium_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_medium_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "aa", "bob", "cc" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_medium_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_medium_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_medium_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_medium_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_heavy_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(1, 3, 5),
                                            { "a", "b", "bob" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_in_degree_evaluation_heavy_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_in_degree_heavy_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_in_degree_evaluation_heavy_effect(*b, new_effect);

    ASSERT_EQ(get_in_degree_evaluation_heavy_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_heavy_effect_observers_are_notified)
{
    constexpr auto new_effect = "bob";
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());
    const effect_evaluation_data new_data { make_ranked(2, 4, 6),
                                            { "aa", "bb", "bob" } };

    EXPECT_CALL(mock, Call(new_data)).Times(1);

    update_out_degree_evaluation_heavy_effect(*b, new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_updating_the_out_degree_heavy_effect_access_it_yields_new_effect)
{
    constexpr auto new_effect = "bob";

    update_out_degree_evaluation_heavy_effect(*b, new_effect);

    ASSERT_EQ(get_out_degree_evaluation_heavy_effect(*b), new_effect);
}

TEST_F(
    given_a_degrees_backend,
    after_restoring_the_in_degree_evaluation_observers_are_notified)
{
    b->connect_to_in_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(initial_data().first)).Times(1);

    restore_in_degree_evaluation(*b);
}

TEST_F(
    given_a_degrees_backend,
    after_restoring_the_in_degree_evaluation_access_to_it_yield_original)
{
    update_in_degree_evaluation(*b, initial_data().second);
    restore_in_degree_evaluation(*b);

    ASSERT_EQ(get_in_degree_effect_evaluation_data(*b), initial_data().first);
}

TEST_F(
    given_a_degrees_backend,
    after_restoring_the_out_degree_evaluation_observers_are_notified)
{
    b->connect_to_out_degree_evaluation(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(initial_data().second)).Times(1);

    restore_out_degree_evaluation(*b);
}

TEST_F(
    given_a_degrees_backend,
    after_restoring_the_out_degree_evaluation_access_to_it_yield_original)
{
    update_out_degree_evaluation(*b, initial_data().first);
    restore_out_degree_evaluation(*b);

    ASSERT_EQ(get_out_degree_effect_evaluation_data(*b), initial_data().second);
}

} // namespace
