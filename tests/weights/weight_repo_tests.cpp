#include "weights/weight_repo.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace weights;

namespace
{

class given_a_weight_repo : public Test
{
protected:
    void SetUp() override
    {
        repo = std::make_unique< weight_repo >(
            weight_repo::hash_table { { "a", 2 }, { "b", 3 } });
    }

    std::unique_ptr< weight_repo > repo;
};

TEST_F(
    given_a_weight_repo,
    querying_weight_of_initial_dependency_results_to_initial_weight)
{
    ASSERT_EQ(repo->get_weight("a"), 2);
    ASSERT_EQ(repo->get_weight("b"), 3);
}

TEST_F(
    given_a_weight_repo,
    querying_weight_of_set_dependency_results_to_assigned_weight)
{
    repo->set_weight("a", 22);

    ASSERT_EQ(repo->get_weight("a"), 22);
}

TEST_F(
    given_a_weight_repo,
    querying_weight_of_set_twice_dependency_results_to_assigned_weight)
{
    repo->set_weight("b", 22);
    repo->set_weight("b", 22);

    ASSERT_EQ(repo->get_weight("b"), 22);
}

} // namespace