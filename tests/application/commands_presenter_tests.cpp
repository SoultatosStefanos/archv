#include "application/commands_presenter.hpp"

#include <boost/signals2/signal.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace application;
using namespace testing;

namespace
{

struct commands_interface
{
    virtual ~commands_interface() = default;

    virtual auto can_undo() const -> bool = 0;
    virtual auto can_redo() const -> bool = 0;

    virtual auto undo() -> void = 0;
    virtual auto redo() -> void = 0;
};

struct mock_commands_t : public commands_interface
{
    mock_commands_t() = default;
    virtual ~mock_commands_t() = default;

    MOCK_METHOD(bool, can_undo, (), (const, override));
    MOCK_METHOD(bool, can_redo, (), (const, override));
    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(void, redo, (), (override));
};

struct editor_interface
{
    using signal = boost::signals2::signal< void() >;
    using slot = signal::slot_type;
    using connection = boost::signals2::connection;
    using pred = std::function< bool() >;

    virtual ~editor_interface() = default;

    virtual auto set_can_undo(pred) -> void = 0;
    virtual auto set_can_redo(pred) -> void = 0;

    virtual auto connect_to_undo(const slot& f) -> connection = 0;
    virtual auto connect_to_redo(const slot& f) -> connection = 0;
};

struct mock_editor_t : public editor_interface
{
    mock_editor_t() = default;
    virtual ~mock_editor_t() = default;

    MOCK_METHOD(void, set_can_undo, (pred), (override));
    MOCK_METHOD(void, set_can_redo, (pred), (override));
    MOCK_METHOD(connection, connect_to_undo, (const slot&), (override));
    MOCK_METHOD(connection, connect_to_redo, (const slot&), (override));
};

using commands_presenter_t
    = commands_presenter< commands_interface, editor_interface >;

class a_new_commands_presenter : public Test
{
};

TEST_F(a_new_commands_presenter, installs_the_editor_accessors)
{
    NiceMock< mock_commands_t > mock_commands;
    NiceMock< mock_editor_t > mock_editor;

    EXPECT_CALL(mock_editor, set_can_undo(_)).Times(1);
    EXPECT_CALL(mock_editor, set_can_redo(_)).Times(1);

    commands_presenter_t(mock_commands, mock_editor);
}

TEST_F(a_new_commands_presenter, installs_the_editor_responses)
{
    NiceMock< mock_commands_t > mock_commands;
    NiceMock< mock_editor_t > mock_editor;

    EXPECT_CALL(mock_editor, connect_to_undo(_)).Times(1);
    EXPECT_CALL(mock_editor, connect_to_redo(_)).Times(1);

    commands_presenter_t(mock_commands, mock_editor);
}

class given_a_commands_presenter : public Test
{
public:
    void SetUp() override
    {
        presenter = std::make_unique< commands_presenter_t >(
            mock_commands, mock_editor);
    }

    NiceMock< mock_commands_t > mock_commands;
    NiceMock< mock_editor_t > mock_editor;
    std::unique_ptr< commands_presenter_t > presenter;
};

TEST_F(
    given_a_commands_presenter, fetch_can_undo_returns_response_from_commands)
{
    ON_CALL(mock_commands, can_undo).WillByDefault(Return(false));
    EXPECT_FALSE(presenter->fetch_can_undo());

    ON_CALL(mock_commands, can_undo).WillByDefault(Return(true));
    EXPECT_TRUE(presenter->fetch_can_undo());
}

TEST_F(
    given_a_commands_presenter, fetch_can_redo_returns_response_from_commands)
{
    ON_CALL(mock_commands, can_redo).WillByDefault(Return(false));

    EXPECT_FALSE(presenter->fetch_can_redo());

    ON_CALL(mock_commands, can_redo).WillByDefault(Return(true));

    EXPECT_TRUE(presenter->fetch_can_redo());
}

TEST_F(given_a_commands_presenter, select_undo_calls_response_from_commands)
{
    EXPECT_CALL(mock_commands, undo).Times(1);

    presenter->select_undo();
}

TEST_F(given_a_commands_presenter, select_redo_calls_response_from_commands)
{
    EXPECT_CALL(mock_commands, redo).Times(1);

    presenter->select_redo();
}

} // namespace
