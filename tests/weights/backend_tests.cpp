#include "weights/backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace testing;

using mock_slot = MockFunction<
    void(weights::backend::dependency_type, weights::backend::weight_type) >;

using nice_mock_slot = NiceMock< mock_slot >;

inline auto initial_config() -> weights::backend::config_data_type
{
    return { { "Type1", 100 }, { "Type2", 50 } };
}

class given_a_weights_backend : public Test
{
public:
    void SetUp() override
    {
        backend = std::make_unique< weights::backend >(initial_config());
    }

protected:
    std::unique_ptr< weights::backend > backend;
    nice_mock_slot mock;
};

TEST_F(given_a_weights_backend, updating_unkown_weight_results_to_noop)
{
    weights::update_weight(*backend, "aaaa", 300);

    ASSERT_TRUE(std::equal(
        std::cbegin(backend->config_data()),
        std::cend(backend->config_data()),
        std::cbegin(backend->get_weight_repo())));
}

TEST_F(given_a_weights_backend, initially_is_configured_via_config)
{
    ASSERT_TRUE(std::equal(
        std::cbegin(backend->config_data()),
        std::cend(backend->config_data()),
        std::cbegin(backend->get_weight_repo())));
}

TEST_F(given_a_weights_backend, updating_a_weight_changes_the_repo)
{
    weights::update_weight(*backend, "Type1", 300);

    ASSERT_EQ(backend->get_weight_repo().get_weight("Type1"), 300);
}

TEST_F(given_a_weights_backend, updating_a_weight_callbacks_the_observers)
{
    backend->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(1);

    weights::update_weight(*backend, "Type1", 300);
}

TEST_F(given_a_weights_backend, export_configs_returns_a_state_copy)
{
    EXPECT_EQ(initial_config(), export_configs(*backend));
}

} // namespace