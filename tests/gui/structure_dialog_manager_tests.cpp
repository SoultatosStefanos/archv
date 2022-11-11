#include "gui/structure_dialog.hpp"
#include "gui/structure_dialog_manager.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

TEST(a_new_structure_dialog_manager, doesnt_manage_any_dialog)
{
    auto manager = gui::structure_dialog_manager();

    ASSERT_FALSE(manager.manages("ClassA"));
    ASSERT_FALSE(manager.manages("X"));
    ASSERT_FALSE(manager.manages("Y"));
    ASSERT_FALSE(manager.manages("FooBar"));
}

TEST(a_new_structure_dialog_manager, has_zero_managed_dialogs)
{
    auto manager = gui::structure_dialog_manager();

    ASSERT_EQ(manager.num_dialogs(), 0);
}

TEST(a_new_structure_dialog_manager, has_zero_active_dialogs)
{
    auto manager = gui::structure_dialog_manager();

    ASSERT_EQ(manager.num_active_dialogs(), 0);
}

TEST(a_new_structure_dialog_manager, does_not_hold_active_dialog)
{
    auto manager = gui::structure_dialog_manager();

    ASSERT_FALSE(manager.is_active("A"));
    ASSERT_FALSE(manager.is_active("A:LL"));
    ASSERT_FALSE(manager.is_active("AS"));
}

class given_a_structure_dialog_manager : public Test
{
protected:
    std::unique_ptr< gui::structure_dialog_manager > manager;

    void SetUp() override
    {
        manager = std::make_unique< gui::structure_dialog_manager >();
    }
};

TEST_F(
    given_a_structure_dialog_manager,
    after_submitting_one_dialog_managed_dialogs_are_one)
{
    manager->submit("A", gui::structure_dialog());

    ASSERT_EQ(manager->num_dialogs(), 1);
}

TEST_F(
    given_a_structure_dialog_manager,
    after_submitting_five_dialog_managed_dialogs_are_five)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->submit("C", gui::structure_dialog());
    manager->submit("D", gui::structure_dialog());
    manager->submit("E", gui::structure_dialog());

    ASSERT_EQ(manager->num_dialogs(), 5);
}

TEST_F(
    given_a_structure_dialog_manager,
    after_submitting_a_dialog_dialog_is_managed)
{
    manager->submit("A", gui::structure_dialog());

    ASSERT_TRUE(manager->manages("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    after_submitting_same_dialog_multiple_times_managed_dialogs_are_one)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());

    ASSERT_EQ(manager->num_dialogs(), 1);
}

TEST_F(
    given_a_structure_dialog_manager,
    after_submitting_same_dialog_multiple_times_dialog_is_managed)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());
    manager->submit("A", gui::structure_dialog());

    ASSERT_TRUE(manager->manages("A"));
}

TEST_F(given_a_structure_dialog_manager, withdraw_initially_does_nothing)
{
    ASSERT_FALSE(manager->manages("A"));

    manager->withdraw("A");

    ASSERT_FALSE(manager->manages("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_multiple_times_initially_does_nothing)
{
    ASSERT_FALSE(manager->manages("A"));
    ASSERT_FALSE(manager->manages("C"));

    manager->withdraw("A");
    manager->withdraw("A");
    manager->withdraw("A");
    manager->withdraw("C");

    ASSERT_FALSE(manager->manages("A"));
    ASSERT_FALSE(manager->manages("C"));
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_after_submitting_means_that_the_dialog_is_not_managed)
{
    manager->submit("A", gui::structure_dialog());
    manager->withdraw("A");

    ASSERT_FALSE(manager->manages("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_after_submitting_means_that_the_num_dialogs_is_0)
{
    manager->submit("A", gui::structure_dialog());
    manager->withdraw("A");

    ASSERT_EQ(manager->num_dialogs(), 0);
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_after_submitting_means_that_the_num_dialogs_is_reduced_by_one)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->withdraw("A");

    ASSERT_EQ(manager->num_dialogs(), 1);
}

TEST_F(
    given_a_structure_dialog_manager, activate_non_managed_dialog_means_death)
{
    ASSERT_DEATH(manager->activate("A"), "");
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_after_submitting_means_that_num_active_dialogs_is_1)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");

    ASSERT_EQ(manager->num_active_dialogs(), 1);
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_after_submitting_means_that_dialog_is_active)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");

    ASSERT_TRUE(manager->is_active("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_same_dialog_multiple_times_after_submitting_means_that_num_active_dialogs_is_1)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");
    manager->activate("A");
    manager->activate("A");
    manager->activate("A");

    ASSERT_EQ(manager->num_active_dialogs(), 1);
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_same_dialog_multiple_times_after_submitting_means_that_dialog_is_active)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");
    manager->activate("A");
    manager->activate("A");
    manager->activate("A");

    ASSERT_TRUE(manager->is_active("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_3_dialogs_after_submitting_then_means_that_num_active_dialogs_is_3)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->submit("C", gui::structure_dialog());
    manager->activate("A");
    manager->activate("B");
    manager->activate("C");

    ASSERT_EQ(manager->num_active_dialogs(), 3);
}

TEST_F(
    given_a_structure_dialog_manager,
    activate_3_dialogs_after_submitting_then_means_that_each_dialog_is_active)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->submit("C", gui::structure_dialog());
    manager->activate("A");
    manager->activate("B");
    manager->activate("C");

    ASSERT_TRUE(manager->is_active("A"));
    ASSERT_TRUE(manager->is_active("B"));
    ASSERT_TRUE(manager->is_active("C"));
}

TEST_F(
    given_a_structure_dialog_manager, deactivate_non_managed_dialog_means_death)
{
    ASSERT_DEATH(manager->deactivate("A"), "");
}

TEST_F(given_a_structure_dialog_manager, deactivate_nonactive_dialog_is_noop)
{
    manager->submit("A", gui::structure_dialog());
    manager->deactivate("A");

    ASSERT_FALSE(manager->is_active("A"));
    ASSERT_EQ(manager->num_active_dialogs(), 0);
}

TEST_F(
    given_a_structure_dialog_manager,
    deactivate_active_dialog_means_that_dialog_is_not_active)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");
    manager->deactivate("A");

    ASSERT_FALSE(manager->is_active("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    deactivate_active_dialog_means_that_num_active_dialogs_is_reduced_by_one)
{
    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->submit("C", gui::structure_dialog());

    manager->activate("A");
    manager->deactivate("A");

    EXPECT_EQ(manager->num_active_dialogs(), 0);

    manager->activate("A");
    manager->activate("B");
    manager->activate("C");
    manager->deactivate("A");

    EXPECT_EQ(manager->num_active_dialogs(), 2);
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_active_dialog_means_that_dialog_is_not_active)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");
    manager->withdraw("A");

    ASSERT_FALSE(manager->is_active("A"));
}

TEST_F(
    given_a_structure_dialog_manager,
    withdraw_active_dialog_means_that_num_active_dialogs_is_reduced_by_one)
{
    manager->submit("A", gui::structure_dialog());
    manager->activate("A");
    manager->withdraw("A");

    EXPECT_EQ(manager->num_active_dialogs(), 0);

    manager->submit("A", gui::structure_dialog());
    manager->submit("B", gui::structure_dialog());
    manager->activate("A");
    manager->activate("B");
    manager->withdraw("A");

    EXPECT_EQ(manager->num_active_dialogs(), 1);
}

} // namespace