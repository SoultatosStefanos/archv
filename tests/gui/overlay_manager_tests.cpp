#include "gui/overlay.hpp"
#include "gui/overlay_manager.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace testing;

TEST(a_new_overlay_manager, doesnt_manage_any_overlay)
{
    auto manager = gui::overlay_manager();

    ASSERT_FALSE(manager.manages("Editor"));
    ASSERT_FALSE(manager.manages("Menu"));
    ASSERT_FALSE(manager.manages("Y"));
    ASSERT_FALSE(manager.manages("z"));
}

TEST(a_new_overlay_manager, has_zero_managed_overlays)
{
    auto manager = gui::overlay_manager();

    ASSERT_EQ(manager.num_overlays(), 0);
}

static constexpr auto mock_id = "mock_overlay";

class mock_overlay_t : public gui::overlay
{
public:
    mock_overlay_t() = default;
    ~mock_overlay_t() override = default;

    auto id() const -> id_type { return mock_id; }

    MOCK_METHOD(bool, visible, (), (const));
    MOCK_METHOD(void, show, (), ());
    MOCK_METHOD(void, hide, (), ());
    MOCK_METHOD(void, render, (), (const));
};

class given_an_overlay_manager : public Test
{
protected:
    using mock_overlay = NiceMock< mock_overlay_t >;
    using mock_func = NiceMock< MockFunction< void(const gui::overlay&) > >;

    std::unique_ptr< gui::overlay_manager > manager;
    mock_func f;

    void SetUp() override
    {
        manager = std::make_unique< gui::overlay_manager >();
    }
};

TEST_F(
    given_an_overlay_manager,
    after_submitting_one_overlay_managed_overlays_are_one)
{
    manager->submit(std::make_unique< mock_overlay >());

    ASSERT_EQ(manager->num_overlays(), 1);
}

TEST_F(
    given_an_overlay_manager,
    after_submitting_five_overlays_with_same_id_managed_overlays_are_one)
{
    manager->submit(std::make_unique< mock_overlay >());
    manager->submit(std::make_unique< mock_overlay >());
    manager->submit(std::make_unique< mock_overlay >());
    manager->submit(std::make_unique< mock_overlay >());
    manager->submit(std::make_unique< mock_overlay >());

    ASSERT_EQ(manager->num_overlays(), 1);
}

TEST_F(
    given_an_overlay_manager,
    after_submitting_an_overlay_manager_manages_overlay)
{
    manager->submit(std::make_unique< mock_overlay >());

    ASSERT_TRUE(manager->manages(mock_id));
}

TEST_F(given_an_overlay_manager, withdraw_initially_does_nothing)
{
    ASSERT_FALSE(manager->manages("A"));

    manager->withdraw("A");

    ASSERT_FALSE(manager->manages("A"));
}

TEST_F(given_an_overlay_manager, withdraw_multiple_times_initially_does_nothing)
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
    given_an_overlay_manager,
    withdraw_after_submitting_means_that_the_overlay_is_not_managed)
{
    manager->submit(std::make_unique< mock_overlay >());
    manager->withdraw(mock_id);

    ASSERT_FALSE(manager->manages(mock_id));
}

TEST_F(
    given_an_overlay_manager,
    withdraw_after_submitting_means_that_the_num_overlays_is_0)
{
    manager->submit(std::make_unique< mock_overlay >());
    manager->withdraw(mock_id);

    ASSERT_EQ(manager->num_overlays(), 0);
}

TEST_F(given_an_overlay_manager, visit_invokes_visitor_initially_zero_times)
{
    EXPECT_CALL(f, Call(_)).Times(0);

    manager->visit(f.AsStdFunction());
}

TEST_F(given_an_overlay_manager, visit_invokes_visitor_num_overlays_times)
{
    manager->submit(std::make_unique< mock_overlay >());

    EXPECT_CALL(f, Call(_)).Times(1);

    manager->visit(f.AsStdFunction());
}

} // namespace