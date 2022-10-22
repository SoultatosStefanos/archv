#include "undo_redo/presenter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

/***********************************************************
 * Mocks                                                   *
 ***********************************************************/

struct commands_interface
{
    virtual ~commands_interface() = default;

    virtual auto can_undo() const -> bool = 0;
    virtual auto can_redo() const -> bool = 0;

    virtual auto undo() -> void = 0;
    virtual auto redo() -> void = 0;
};

struct commands_mock : public commands_interface
{
    commands_mock() = default;
    virtual ~commands_mock() = default;

    MOCK_METHOD(bool, can_undo, (), (const, override));
    MOCK_METHOD(bool, can_redo, (), (const, override));
    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(void, redo, (), (override));
};

struct dummy_view
{
    using slot = std::function< void() >;
    using pred = std::function< bool() >;

    auto set_can_undo(const pred&) -> void { }
    auto set_can_redo(const pred&) -> void { }

    auto connect_to_undo(const slot&) -> void { }
    auto connect_to_redo(const slot&) -> void { }
};

using presenter = undo_redo::presenter< dummy_view, commands_interface >;

/***********************************************************
 * Tests                                                   *
 ***********************************************************/

class given_a_commands_presenter : public Test
{
public:
    void SetUp() override { pres = std::make_unique< presenter >(cmds, view); }

    NiceMock< commands_mock > cmds;
    dummy_view view;
    std::unique_ptr< presenter > pres;
};

TEST_F(
    given_a_commands_presenter, fetch_can_undo_returns_response_from_commands)
{
    ON_CALL(cmds, can_undo).WillByDefault(Return(false));
    EXPECT_FALSE(pres->fetch_can_undo());

    ON_CALL(cmds, can_undo).WillByDefault(Return(true));
    EXPECT_TRUE(pres->fetch_can_undo());
}

TEST_F(
    given_a_commands_presenter, fetch_can_redo_returns_response_from_commands)
{
    ON_CALL(cmds, can_redo).WillByDefault(Return(false));

    EXPECT_FALSE(pres->fetch_can_redo());

    ON_CALL(cmds, can_redo).WillByDefault(Return(true));

    EXPECT_TRUE(pres->fetch_can_redo());
}

TEST_F(given_a_commands_presenter, select_undo_calls_response_from_commands)
{
    EXPECT_CALL(cmds, undo).Times(1);

    pres->select_undo();
}

TEST_F(given_a_commands_presenter, select_redo_calls_response_from_commands)
{
    EXPECT_CALL(cmds, redo).Times(1);

    pres->select_redo();
}

} // namespace
