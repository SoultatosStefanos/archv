#include "scaling/backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace scaling;

namespace
{

using mock_t = MockFunction< void(const factor&) >;
using nice_mock_t = NiceMock< mock_t >;

class given_a_scaling_backend : public Test
{
protected:
    void SetUp() override
    {
        b = std::make_unique< backend >(
            backend::config_data_type { { "f", make_x_factor(10, true) } });
    }

    std::unique_ptr< backend > b;
    nice_mock_t mock;
};

TEST_F(given_a_scaling_backend, queries_on_applied_dims_are_valid)
{
    ASSERT_TRUE(is_factor_applied_on_x(*b, "f"));
    ASSERT_FALSE(is_factor_applied_on_y(*b, "f"));
    ASSERT_FALSE(is_factor_applied_on_z(*b, "f"));
}

TEST_F(given_a_scaling_backend, queries_on_wether_factor_is_enabled_are_valid)
{
    ASSERT_TRUE(is_factor_enabled(*b, "f"));
}

TEST_F(given_a_scaling_backend, queries_on_factor_baseline_are_valid)
{
    ASSERT_EQ(get_factor_baseline(*b, "f"), 10);
}

TEST_F(given_a_scaling_backend, factor_applied_dims_can_be_set)
{
    apply_factor_on_y_axis(*b, "f");

    ASSERT_TRUE(is_factor_applied_on_x(*b, "f"));
    ASSERT_TRUE(is_factor_applied_on_y(*b, "f"));

    apply_factor_on_z_axis(*b, "f");

    ASSERT_TRUE(is_factor_applied_on_x(*b, "f"));
    ASSERT_TRUE(is_factor_applied_on_y(*b, "f"));
    ASSERT_TRUE(is_factor_applied_on_z(*b, "f"));
}

TEST_F(given_a_scaling_backend, setting_factor_applied_dims_notifies_observers)
{
    b->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(make_xy_factor(10, true))).Times(1);

    apply_factor_on_y_axis(*b, "f");
}

TEST_F(given_a_scaling_backend, factor_applied_dims_can_be_unset)
{
    apply_factor_on_x_axis(*b, "f");
    EXPECT_TRUE(is_factor_applied_on_x(*b, "f"));
    deny_factor_on_x_axis(*b, "f");
    ASSERT_FALSE(is_factor_applied_on_x(*b, "f"));

    apply_factor_on_y_axis(*b, "f");
    EXPECT_TRUE(is_factor_applied_on_y(*b, "f"));
    deny_factor_on_y_axis(*b, "f");
    ASSERT_FALSE(is_factor_applied_on_y(*b, "f"));

    apply_factor_on_z_axis(*b, "f");
    EXPECT_TRUE(is_factor_applied_on_z(*b, "f"));
    deny_factor_on_z_axis(*b, "f");
    ASSERT_FALSE(is_factor_applied_on_z(*b, "f"));
}

TEST_F(
    given_a_scaling_backend, unsetting_factor_applied_dims_notifies_observers)
{
    b->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(make_nil_factor(10, true))).Times(1);

    deny_factor_on_x_axis(*b, "f");
}

TEST_F(given_a_scaling_backend, factor_enabled_can_be_set_unset)
{
    EXPECT_TRUE(is_factor_enabled(*b, "f"));

    disable_factor(*b, "f");

    ASSERT_FALSE(is_factor_enabled(*b, "f"));

    enable_factor(*b, "f");

    ASSERT_TRUE(is_factor_enabled(*b, "f"));
}

TEST_F(given_a_scaling_backend, setting_factor_enabled_notifies_observers)
{
    b->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(make_x_factor(10, false))).Times(1);

    disable_factor(*b, "f");
}

TEST_F(given_a_scaling_backend, factor_baseline_can_be_set)
{
    set_factor_baseline(*b, "f", 300);

    ASSERT_EQ(get_factor_baseline(*b, "f"), 300);
}

TEST_F(given_a_scaling_backend, setting_factor_baseline_notifies_observers)
{
    b->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(make_x_factor(300, true))).Times(1);

    set_factor_baseline(*b, "f", 300);
}

} // namespace