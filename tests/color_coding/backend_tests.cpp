#include "color_coding/backend.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace color_coding;
using namespace testing;

using mock_slot
    = MockFunction< void(backend::dependency_type, backend::color_code_type) >;

using nice_mock_slot = NiceMock< mock_slot >;

using rgba = color_code::rgba_type;

inline auto initial_config() -> backend::config_data_type
{
    return { { "Type1", color_code({ 1, 1, 1, 1 }) },
             { "Type2", color_code({ 2, 2, 2, 2 }) } };
}

class given_a_color_coding_backend : public Test
{
public:
    void SetUp() override
    {
        inst = std::make_unique< backend >(initial_config());
    }

protected:
    std::unique_ptr< backend > inst;
    nice_mock_slot mock;
};

TEST_F(given_a_color_coding_backend, updating_unkown_color_code_results_to_noop)
{
    update_color_code(*inst, "aaaa", color_code({ 2, 20, 22, 12 }));

    ASSERT_TRUE(std::equal(
        std::cbegin(inst->config_data()),
        std::cend(inst->config_data()),
        std::cbegin(inst->repo())));
}

TEST_F(given_a_color_coding_backend, initially_is_configured_via_config)
{
    ASSERT_TRUE(std::equal(
        std::cbegin(inst->config_data()),
        std::cend(inst->config_data()),
        std::cbegin(inst->repo())));
}

TEST_F(given_a_color_coding_backend, updating_a_color_code_changes_the_repo)
{
    update_color_code(*inst, "Type1", color_code({ { 2, 20, 22, 12 } }, true));

    ASSERT_EQ(
        get_color_code(*inst, "Type1"),
        color_code({ { 2, 20, 22, 12 } }, true));
}

TEST_F(given_a_color_coding_backend, updating_a_color_changes_the_repo)
{
    update_color(*inst, "Type1", rgba({ 2, 20, 22, 12 }));

    ASSERT_EQ(get_color(*inst, "Type1"), rgba({ 2, 20, 22, 12 }));
}

TEST_F(
    given_a_color_coding_backend,
    updating_a_color_activeness_changes_the_repo)
{
    update_color_active(*inst, "Type1", false);

    EXPECT_FALSE(is_color_active(*inst, "Type1"));

    update_color_active(*inst, "Type1", true);

    EXPECT_TRUE(is_color_active(*inst, "Type1"));

    EXPECT_EQ(get_color(*inst, "Type1"), rgba({ 1, 1, 1, 1 }));
}

TEST_F(
    given_a_color_coding_backend,
    updating_a_color_code_callbacks_the_observers)
{
    inst->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(1);

    update_color_code(*inst, "Type1", color_code({ { 2, 20, 22, 12 } }, true));
}

TEST_F(given_a_color_coding_backend, updating_a_color_callbacks_the_observers)
{
    inst->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(1);

    update_color(*inst, "Type1", rgba({ 2, 20, 22, 12 }));
}

TEST_F(
    given_a_color_coding_backend,
    updating_a_color_activeness_callbacks_the_observers)
{
    inst->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(1);

    update_color_active(*inst, "Type1", false);
}

TEST_F(given_a_color_coding_backend, restore_defaults_reverts_state_to_initial)
{
    update_color_code(*inst, "Type1", color_code({ { 2, 20, 22, 12 } }, false));

    EXPECT_EQ(
        get_color_code(*inst, "Type1"),
        color_code({ { 2, 20, 22, 12 } }, false));

    restore_defaults(*inst);

    ASSERT_TRUE(std::equal(
        std::cbegin(inst->config_data()),
        std::cend(inst->config_data()),
        std::cbegin(inst->repo())));
}

TEST_F(
    given_a_color_coding_backend,
    restore_defaults_notifies_callbacks_for_each_dependency)
{
    inst->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(_, _)).Times(2);

    restore_defaults(*inst);
}

TEST_F(given_a_color_coding_backend, export_configs_returns_a_state_copy)
{
    EXPECT_EQ(initial_config(), export_configs(*inst));
}

} // namespace