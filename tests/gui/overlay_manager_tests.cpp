#include "gui/overlay_manager.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

TEST(an_overlay_manager_initially, doesnt_manage_any_popup)
{
    gui::overlay_manager overlays;
    gui::structure_popup p1, p2, p3;

    EXPECT_FALSE(overlays.manages(&p1));
    EXPECT_FALSE(overlays.manages(&p2));
    EXPECT_FALSE(overlays.manages(&p3));
}

TEST(an_overlay_manager_initially, has_num_popups_zero)
{
    gui::overlay_manager overlays;

    EXPECT_EQ(overlays.num_popups(), 0);
}

class given_an_overlay_manager : public Test
{
protected:
    void SetUp() override
    {
        overlays = std::make_unique< gui::overlay_manager >();
    }

    std::unique_ptr< gui::overlay_manager > overlays;
    gui::structure_popup p1, p2, p3;
};

TEST_F(given_an_overlay_manager, when_submitting_a_popup_then_manages)
{
    overlays->submit(&p1);

    EXPECT_TRUE(overlays->manages(&p1));
}

TEST_F(given_an_overlay_manager, when_submitting_a_popup_then_num_popups_is_1)
{
    overlays->submit(&p1);

    EXPECT_EQ(overlays->num_popups(), 1);
}

TEST_F(
    given_an_overlay_manager, when_submitting_the_same_popup_twice_then_manages)
{
    overlays->submit(&p1);
    overlays->submit(&p1);

    EXPECT_TRUE(overlays->manages(&p1));
}

TEST_F(
    given_an_overlay_manager,
    when_submitting_with_the_same_popup_twice_then_num_popups_is_1)
{
    overlays->submit(&p1);
    overlays->submit(&p1);

    EXPECT_EQ(overlays->num_popups(), 1);
}

TEST_F(
    given_an_overlay_manager,
    when_submitting_two_popups_with_dif_ids_then_manages_both_popups)
{
    overlays->submit(&p1);
    overlays->submit(&p2);

    EXPECT_TRUE(overlays->manages(&p1));
    EXPECT_TRUE(overlays->manages(&p2));
}

TEST_F(
    given_an_overlay_manager,
    when_submitting_two_dif_popups_then_num_popups_is_2)
{
    overlays->submit(&p1);
    overlays->submit(&p2);

    EXPECT_EQ(overlays->num_popups(), 2);
}

TEST_F(given_an_overlay_manager, withdraw_initially_does_nothing)
{
    EXPECT_NO_THROW(overlays->withdraw(&p1));
    EXPECT_EQ(overlays->num_popups(), 0);
    EXPECT_FALSE(overlays->manages(&p1));
}

TEST_F(
    given_an_overlay_manager,
    withdraw_after_submit_means_that_popup_is_no_longer_managed)
{
    overlays->submit(&p1);

    overlays->withdraw(&p1);

    EXPECT_FALSE(overlays->manages(&p1));
}

TEST_F(
    given_an_overlay_manager,
    withdraw_after_1_submit_means_that_num_popups_is_0)
{
    overlays->submit(&p1);

    overlays->withdraw(&p1);

    EXPECT_EQ(overlays->num_popups(), 0);
}

} // namespace