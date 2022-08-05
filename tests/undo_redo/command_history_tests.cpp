#include "undo_redo/command.hpp"
#include "undo_redo/command_history.hpp"
#include "utility/random.hpp"

#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace testing;
using namespace utility;
using namespace undo_redo;

class mock_command : public command
{
public:
    virtual ~mock_command() override = default;

    MOCK_METHOD(void, execute, (), (override));

    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(void, redo, (), (override));

    MOCK_METHOD(std::unique_ptr< command >, clone, (), (const, override));
};

class undo_redo : public Test
{
public:
    using nice_mock_command = NiceMock< mock_command >;

    void SetUp() override
    {
        cmd_history = std::make_unique< command_history >();
    }

protected:
    std::unique_ptr< command_history > cmd_history;
};

TEST_F(undo_redo, undoing_initially_does_nothing)
{
    ASSERT_NO_FATAL_FAILURE(cmd_history->undo());
}

TEST_F(undo_redo, redoing_initially_does_nothing)
{
    ASSERT_NO_FATAL_FAILURE(cmd_history->redo());
}

TEST_F(undo_redo, executing_a_cmd_through_it_executes_cmd)
{
    auto cmd = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd, execute).Times(1);

    cmd_history->execute(std::move(cmd));
}

TEST_F(undo_redo, undoing_cmd_after_executing_it_undoes_it)
{
    auto cmd = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, undo).Times(1);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
}

TEST_F(undo_redo, redoing_cmd_after_executing_it_does_nothing)
{
    auto cmd = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, redo).Times(0);

    cmd_history->execute(std::move(cmd));
    cmd_history->redo();
}

TEST_F(undo_redo, redoing_cmd_after_undoing_it_redoes_it)
{
    auto cmd = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, undo).Times(1);
    EXPECT_CALL(*cmd, redo).Times(1);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
    cmd_history->redo();
}

TEST_F(undo_redo, executes_cmds_in_order)
{
    auto cmd1 = std::make_unique< nice_mock_command >();
    auto cmd2 = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd1, execute).Times(1);
    cmd_history->execute(std::move(cmd1));

    EXPECT_CALL(*cmd2, execute).Times(1);
    cmd_history->execute(std::move(cmd2));
}

TEST_F(undo_redo, undoes_cmds_in_lifo)
{
    auto cmd1 = std::make_unique< nice_mock_command >();
    auto cmd2 = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd2, undo).Times(1);
    EXPECT_CALL(*cmd1, undo).Times(0);

    cmd_history->execute(std::move(cmd1));
    cmd_history->execute(std::move(cmd2));
    cmd_history->undo();
}

TEST_F(undo_redo, redoes_cmds_in_lifo)
{
    auto cmd1 = std::make_unique< nice_mock_command >();
    auto cmd2 = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd2, undo).Times(1);
    EXPECT_CALL(*cmd2, redo).Times(1);
    EXPECT_CALL(*cmd1, undo).Times(0);
    EXPECT_CALL(*cmd1, redo).Times(0);

    cmd_history->execute(std::move(cmd1));
    cmd_history->execute(std::move(cmd2));
    cmd_history->undo();
    cmd_history->redo();
}

TEST_F(undo_redo, undoing_n_commands_after_executing_them_undoes_them_all)
{
    const auto n = urandom(2, 10);
    std::vector< std::unique_ptr< nice_mock_command > > mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique< nice_mock_command >();

    for (const auto& cmd : mock_commands)
    {
        EXPECT_CALL(*cmd, execute).Times(1);
        EXPECT_CALL(*cmd, undo).Times(1);
    }

    for (auto& cmd : mock_commands)
        cmd_history->execute(std::move(cmd));

    for (auto i = 0; i < n; ++i)
        cmd_history->undo();
}

TEST_F(undo_redo, redoing_n_commands_after_undoing_them_redoes_them_all)
{
    const auto n = urandom(2, 10);
    std::vector< std::unique_ptr< nice_mock_command > > mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique< nice_mock_command >();

    for (const auto& cmd : mock_commands)
    {
        EXPECT_CALL(*cmd, execute).Times(1);
        EXPECT_CALL(*cmd, undo).Times(1);
        EXPECT_CALL(*cmd, redo).Times(1);
    }

    for (auto& cmd : mock_commands)
        cmd_history->execute(std::move(cmd));

    for (auto i = 0; i < n; ++i)
        cmd_history->undo();

    for (auto i = 0; i < n; ++i)
        cmd_history->redo();
}

TEST_F(undo_redo, after_undoing_and_executing_once_redo_does_nothing)
{
    auto cmd = std::make_unique< nice_mock_command >();
    auto cmd2 = std::make_unique< nice_mock_command >();

    EXPECT_CALL(*cmd, redo).Times(0);
    EXPECT_CALL(*cmd2, redo).Times(0);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
    cmd_history->execute(std::move(cmd2));
    cmd_history->redo();
}

TEST_F(undo_redo, after_undoing_n_times_and_executing_once_redo_does_nothing)
{
    const auto n = urandom(2, 10);
    std::vector< std::unique_ptr< nice_mock_command > > mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique< nice_mock_command >();

    for (const auto& cmd : mock_commands)
    {
        EXPECT_CALL(*cmd, redo).Times(0);
    }

    for (auto& cmd : mock_commands)
        cmd_history->execute(std::move(cmd));

    for (auto i = 0; i < n; ++i)
        cmd_history->undo();

    auto cmd2 = std::make_unique< nice_mock_command >();
    cmd_history->execute(std::move(cmd2));
    cmd_history->redo();
}

} // namespace