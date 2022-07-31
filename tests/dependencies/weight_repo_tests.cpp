#include "dependencies/weight_repo.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace dependencies;

namespace
{

class given_a_weight_repo : public Test
{
protected:
    void SetUp() override { repo = std::make_unique<weight_repo>(); }

    std::unique_ptr<weight_repo> repo;
};

TEST_F(given_a_weight_repo,
       querying_weight_of_unset_dependency_results_to_death)
{
    EXPECT_DEATH(repo->get_weight("any"), "");
}

TEST_F(given_a_weight_repo,
       querying_weight_of_set_dependency_results_to_assigned_weight)
{
    static constexpr auto dependency = "aaa";
    static constexpr auto weight = 22;

    repo->set_weight(dependency, weight);

    ASSERT_EQ(repo->get_weight(dependency), weight);
}

TEST(a_weight_repo, can_be_constructed_from_a_hash_table)
{
    weight_repo::hash_table table{{"a", 2}, {"b", 3}};
    weight_repo expected;
    expected.set_weight("a", 2);
    expected.set_weight("b", 3);

    weight_repo actual = weight_repo(table);

    ASSERT_EQ(actual, expected);
}

} // namespace