#include "view/state.hpp"
#include "view/state_machine.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace view;
using namespace testing;

namespace
{

class mock_state : public state
{
public:
    mock_state() = default;
    virtual ~mock_state() override = default;

    MOCK_METHOD(void, enter, (), (override));
    MOCK_METHOD(void, exit, (), (override));
    MOCK_METHOD(void, pause, (), (override));
    MOCK_METHOD(void, resume, (), (override));
};

class given_a_state_machine : public Test
{
protected:
    void SetUp() override { sm = std::make_unique< state_machine >(); }

    std::unique_ptr< state_machine > sm;
    NiceMock< mock_state > mock;
    NiceMock< mock_state > mock2;
};

TEST_F(given_a_state_machine, initially_is_not_started)
{
    ASSERT_FALSE(sm->started());
}

TEST_F(given_a_state_machine, has_initially_no_active_state)
{
    ASSERT_FALSE(sm->has_active_state());
}

TEST_F(given_a_state_machine, after_start_is_started)
{
    sm->start(&mock);

    ASSERT_TRUE(sm->started());
}

TEST_F(given_a_state_machine, after_start_has_active_state)
{
    sm->start(&mock);

    ASSERT_TRUE(sm->has_active_state());
}

TEST_F(
    given_a_state_machine, after_start_its_active_state_is_the_one_passed_to_it)
{
    sm->start(&mock);

    ASSERT_EQ(sm->get_active_state(), &mock);
}

TEST_F(given_a_state_machine, after_transition_has_active_state)
{
    sm->start(&mock);

    sm->transition_to(&mock2);

    ASSERT_TRUE(sm->has_active_state());
}

TEST_F(given_a_state_machine, after_transition_the_active_state_is_the_last_one)
{
    sm->start(&mock);

    sm->transition_to(&mock2);

    ASSERT_EQ(sm->get_active_state(), &mock2);
}

TEST_F(
    given_a_state_machine,
    on_transition_the_pause_function_is_called_on_the_previous_state)
{
    sm->start(&mock);

    EXPECT_CALL(mock, pause).Times(1);

    sm->transition_to(&mock2);
}

TEST_F(
    given_a_state_machine,
    on_transition_the_enter_function_is_called_on_the_new_state)
{
    sm->start(&mock);

    EXPECT_CALL(mock2, enter).Times(1);

    sm->transition_to(&mock2);
}

TEST_F(given_a_state_machine, a_commit_transition_takes_us_to_the_next_state)
{
    sm->start(&mock);
    sm->commit_transition_to(&mock2);

    ASSERT_TRUE(sm->has_active_state());
    ASSERT_EQ(sm->get_active_state(), &mock2);
}

TEST_F(
    given_a_state_machine,
    a_commit_transition_calls_the_curr_states_exit_function)
{
    sm->start(&mock);

    EXPECT_CALL(mock, exit).Times(1);

    sm->commit_transition_to(&mock2);
}

TEST_F(
    given_a_state_machine,
    a_commit_transition_calls_the_next_states_enter_function)
{
    sm->start(&mock);

    EXPECT_CALL(mock2, enter).Times(1);

    sm->commit_transition_to(&mock2);
}

TEST_F(given_a_state_machine, a_fallback_after_starting_causes_no_active_state)
{
    sm->start(&mock);
    sm->fallback();

    ASSERT_FALSE(sm->has_active_state());
}

TEST_F(
    given_a_state_machine,
    a_fallback_after_changing_state_reverts_to_previous_state)
{
    sm->start(&mock);
    sm->transition_to(&mock2);

    sm->fallback();

    ASSERT_TRUE(sm->has_active_state());
    ASSERT_EQ(sm->get_active_state(), &mock);
}

TEST_F(
    given_a_state_machine,
    two_transitions_and_one_fallback_takes_us_back_to_second_state)
{
    sm->start(&mock);
    sm->transition_to(&mock2);
    sm->transition_to(&mock);

    sm->fallback();

    ASSERT_TRUE(sm->has_active_state());
    ASSERT_EQ(sm->get_active_state(), &mock2);
}

TEST_F(given_a_state_machine, a_fallback_calls_the_curr_states_exit_function)
{
    sm->start(&mock);
    sm->transition_to(&mock2);

    EXPECT_CALL(mock2, exit).Times(1);

    sm->fallback();
}

TEST_F(
    given_a_state_machine,
    a_fallback_after_changing_state_calls_the_previous_states_resume_function)
{
    sm->start(&mock);
    sm->transition_to(&mock2);

    EXPECT_CALL(mock, resume).Times(1);

    sm->fallback();
}

} // namespace