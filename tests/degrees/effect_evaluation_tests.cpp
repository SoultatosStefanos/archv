#include "degrees/effect_evaluation.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace degrees;
using namespace testing;

namespace
{

TEST(
    a_degrees_effect_evaluation,
    given_degree_below_light_threshold_returns_empty_option)
{
    constexpr auto thresholds = make_ranked(1, 2, 3);
    const auto effects = make_ranked< std::string >("a", "n", "y");
    const auto evaluation_data = effect_evaluation_data(thresholds, effects);

    ASSERT_EQ(evaluate(0, evaluation_data), null_effect);
}

TEST(
    a_degrees_effect_evaluation,
    given_degree_above_or_equal_light_and_below_medium_threshold_returns_light_option)
{
    constexpr auto thresholds = make_ranked(1, 3, 4);
    const auto effects = make_ranked< std::string >("a", "n", "y");
    const auto evaluation_data = effect_evaluation_data(thresholds, effects);

    ASSERT_EQ(evaluate(1, evaluation_data), effects.light);
    ASSERT_EQ(evaluate(2, evaluation_data), effects.light);
}

TEST(
    a_degrees_effect_evaluation,
    given_degree_above_or_equal_medium_and_below_heavy_threshold_returns_medium_option)
{
    constexpr auto thresholds = make_ranked(1, 3, 7);
    const auto effects = make_ranked< std::string >("a", "n", "y");
    const auto evaluation_data = effect_evaluation_data(thresholds, effects);

    ASSERT_EQ(evaluate(3, evaluation_data), effects.medium);
    ASSERT_EQ(evaluate(5, evaluation_data), effects.medium);
}

TEST(
    a_degrees_effect_evaluation,
    given_degree_above_or_equal_heavy_threshold_returns_heavy_option)
{
    constexpr auto thresholds = make_ranked(1, 3, 7);
    const auto effects = make_ranked< std::string >("a", "n", "y");
    const auto evaluation_data = effect_evaluation_data(thresholds, effects);

    ASSERT_EQ(evaluate(7, evaluation_data), effects.heavy);
    ASSERT_EQ(
        evaluate(std::numeric_limits< degree_t >::max(), evaluation_data),
        effects.heavy);
}

} // namespace