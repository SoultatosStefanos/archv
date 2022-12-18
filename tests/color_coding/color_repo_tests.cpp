#include "color_coding/color_repo.hpp"

#include <gtest/gtest.h>

using namespace testing;
using namespace color_coding;

namespace
{

class given_a_color_repo : public Test
{
protected:
    void SetUp() override
    {
        repo = std::make_unique< color_repo >(
            color_repo::hash_table { { "a", color_code({ 1, 1, 1, 1 }) },
                                     { "b", color_code({ 255, 1, 1, 1 }) } });
    }

    std::unique_ptr< color_repo > repo;
};

TEST_F(
    given_a_color_repo,
    querying_color_of_initial_dependency_results_to_initial_color)
{
    ASSERT_EQ(repo->get_color("a"), color_code({ 1, 1, 1, 1 }));
    ASSERT_EQ(repo->get_color("b"), color_code({ 255, 1, 1, 1 }));
}

TEST_F(
    given_a_color_repo,
    querying_color_of_set_dependency_results_to_assigned_color)
{
    repo->set_color("a", color_code({ 0, 0, 0, 0 }));

    ASSERT_EQ(repo->get_color("a"), color_code({ 0, 0, 0, 0 }));
}

TEST_F(
    given_a_color_repo,
    querying_color_of_set_twice_dependency_results_to_assigned_color)
{
    repo->set_color("b", color_code({ 1, 0, 0, 0 }));
    repo->set_color("b", color_code({ 1, 0, 0, 0 }));

    ASSERT_EQ(repo->get_color("b"), color_code({ 1, 0, 0, 0 }));
}

TEST_F(given_a_color_repo, dependencies_can_be_paired_with_inactive_color_codes)
{
    repo->set_color("b", color_code({ { 1, 0, 0, 0 } }, false));

    ASSERT_EQ(repo->get_color("b"), color_code({ { 1, 0, 0, 0 } }, false));
}

} // namespace