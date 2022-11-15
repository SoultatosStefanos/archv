#include "gui/popup_holder.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

TEST(a_popup_holder_initially, doesnt_contain_any_popup)
{
    gui::popup_holder popups;

    EXPECT_FALSE(popups.contains("ClassA"));
    EXPECT_FALSE(popups.contains("Foor"));
    EXPECT_FALSE(popups.contains("Alu"));
}

TEST(a_popup_holder_initially, has_size_zero)
{
    gui::popup_holder popups;

    EXPECT_EQ(popups.size(), 0);
}

class given_a_popup_holder : public Test
{
protected:
    void SetUp() override { popups = std::make_unique< gui::popup_holder >(); }

    std::unique_ptr< gui::popup_holder > popups;
};

TEST_F(given_a_popup_holder, when_inserting_a_popup_then_contains)
{
    popups->insert("ClassX", gui::structure_popup());

    EXPECT_TRUE(popups->contains("ClassX"));
}

TEST_F(given_a_popup_holder, when_inserting_a_popup_then_size_is_1)
{
    popups->insert("ClassX", gui::structure_popup());

    EXPECT_EQ(popups->size(), 1);
}

TEST_F(
    given_a_popup_holder, when_inserting_with_the_same_id_twice_then_contains)
{
    popups->insert("ClassX", gui::structure_popup());
    popups->insert("ClassX", gui::structure_popup());

    EXPECT_TRUE(popups->contains("ClassX"));
}

TEST_F(
    given_a_popup_holder, when_inserting_with_the_same_id_twice_then_size_is_1)
{
    popups->insert("ClassX", gui::structure_popup());
    popups->insert("ClassX", gui::structure_popup());

    EXPECT_EQ(popups->size(), 1);
}

TEST_F(
    given_a_popup_holder,
    when_inserting_two_popups_with_dif_ids_then_contains_both_popups)
{
    popups->insert("Foo", gui::structure_popup());
    popups->insert("Bar", gui::structure_popup());

    EXPECT_TRUE(popups->contains("Foo"));
    EXPECT_TRUE(popups->contains("Bar"));
}

TEST_F(
    given_a_popup_holder, when_inserting_two_popups_with_dif_ids_then_size_is_2)
{
    popups->insert("Foo", gui::structure_popup());
    popups->insert("Bar", gui::structure_popup());

    EXPECT_EQ(popups->size(), 2);
}

TEST_F(given_a_popup_holder, remove_initially_does_nothing)
{
    EXPECT_NO_THROW(popups->remove("Foo"));
    EXPECT_EQ(popups->size(), 0);
    EXPECT_FALSE(popups->contains("Foo"));
}

TEST_F(
    given_a_popup_holder,
    remove_after_insert_means_that_popup_is_no_longer_containd)
{
    popups->insert("Buzz", gui::structure_popup());

    popups->remove("Buzz");

    EXPECT_FALSE(popups->contains("Buzz"));
}

TEST_F(given_a_popup_holder, remove_after_1_insert_means_that_size_is_0)
{
    popups->insert("Buzz", gui::structure_popup());

    popups->remove("Buzz");

    EXPECT_EQ(popups->size(), 0);
}

TEST_F(given_a_popup_holder, clear_initially_does_nothing)
{
    EXPECT_NO_THROW(popups->clear());
    EXPECT_EQ(popups->size(), 0);
    EXPECT_FALSE(popups->contains("Foo"));
}

TEST_F(given_a_popup_holder, after_2_inserts_and_clear_then_size_is_0)
{
    popups->insert("Buzz", gui::structure_popup());
    popups->insert("HAA", gui::structure_popup());

    popups->clear();

    EXPECT_EQ(popups->size(), 0);
}

TEST_F(given_a_popup_holder, after_2_inserts_and_clear_then_contains_none)
{
    popups->insert("Buzz", gui::structure_popup());
    popups->insert("HAA", gui::structure_popup());

    popups->clear();

    EXPECT_FALSE(popups->contains("Buzz"));
    EXPECT_FALSE(popups->contains("HAA"));
}

} // namespace