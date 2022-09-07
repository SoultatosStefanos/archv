#include "dependencies/backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace testing;

using mock_slot = MockFunction< void(const std::string&, double) >;
using nice_mock_slot = NiceMock< mock_slot >;

inline auto initial_config() -> dependencies::backend::config_data_type
{
    return { { "Type1", 100 }, { "Type2", 50 } };
}

class given_a_dependencies_backend : public Test
{
public:
    void SetUp() override
    {
        backend = std::make_unique< dependencies::backend >(initial_config());
    }

protected:
    std::unique_ptr< dependencies::backend > backend;
    nice_mock_slot mock;
};

TEST_F(given_a_dependencies_backend, initially_is_configured_via_config)
{
    ASSERT_TRUE(std::equal(
        std::cbegin(backend->config_data()),
        std::cend(backend->config_data()),
        std::cbegin(backend->get_weight_repo())));
}

TEST_F(given_a_dependencies_backend, updating_a_weight_changes_the_repo)
{
    dependencies::update_weight(*backend, "Type1", 300);

    ASSERT_EQ(backend->get_weight_repo().get_weight("Type1"), 300);
}

TEST_F(given_a_dependencies_backend, updating_a_weight_callbacks_the_observers)
{
    backend->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(1);

    dependencies::update_weight(*backend, "Type1", 300);
}

} // namespace