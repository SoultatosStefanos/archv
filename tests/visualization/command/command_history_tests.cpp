#include "utility/all.hpp"
#include "visualization/command/all.hpp"

#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace visualization::command;
using namespace utility;

class mock_command : public command
{
public:
    virtual ~mock_command() override = default;

    MOCK_METHOD(void, execute, (), (override));

    MOCK_METHOD(void, undo, (), (override));
    MOCK_METHOD(void, redo, (), (override));

    MOCK_METHOD(std::unique_ptr<command>, clone, (), (const, override));
};

class Given_a_command_history : public testing::Test
{
public:
    void SetUp() override { cmd_history = std::make_unique<command_history>(); }

protected:
    std::unique_ptr<command_history> cmd_history;
};

TEST_F(Given_a_command_history, Undoing_initially_does_nothing)
{
    ASSERT_NO_FATAL_FAILURE(cmd_history->undo());
}

TEST_F(Given_a_command_history, Redoing_initially_does_nothing)
{
    ASSERT_NO_FATAL_FAILURE(cmd_history->redo());
}

TEST_F(Given_a_command_history, Executing_a_cmd_through_it_executes_cmd)
{
    auto cmd = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd, execute).Times(1);

    cmd_history->execute(std::move(cmd));
}

TEST_F(Given_a_command_history, Undoing_cmd_after_executing_it_undoes_it)
{
    auto cmd = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, undo).Times(1);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
}

TEST_F(Given_a_command_history, Redoing_cmd_after_executing_it_does_nothing)
{
    auto cmd = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, redo).Times(0);

    cmd_history->execute(std::move(cmd));
    cmd_history->redo();
}

TEST_F(Given_a_command_history, Redoing_cmd_after_undoing_it_redoes_it)
{
    auto cmd = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd, execute).Times(1);
    EXPECT_CALL(*cmd, undo).Times(1);
    EXPECT_CALL(*cmd, redo).Times(1);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
    cmd_history->redo();
}

TEST_F(Given_a_command_history, Executes_cmds_in_order)
{
    auto cmd1 = std::make_unique<mock_command>();
    auto cmd2 = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd1, execute).Times(1);
    cmd_history->execute(std::move(cmd1));

    EXPECT_CALL(*cmd2, execute).Times(1);
    cmd_history->execute(std::move(cmd2));
}

TEST_F(Given_a_command_history, Undoes_cmds_in_lifo)
{
    auto cmd1 = std::make_unique<mock_command>();
    auto cmd2 = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd2, undo).Times(1);
    EXPECT_CALL(*cmd1, undo).Times(0);

    cmd_history->execute(std::move(cmd1));
    cmd_history->execute(std::move(cmd2));
    cmd_history->undo();
}

TEST_F(Given_a_command_history, Redoes_cmds_in_lifo)
{
    auto cmd1 = std::make_unique<mock_command>();
    auto cmd2 = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd2, undo).Times(1);
    EXPECT_CALL(*cmd2, redo).Times(1);
    EXPECT_CALL(*cmd1, undo).Times(0);
    EXPECT_CALL(*cmd1, redo).Times(0);

    cmd_history->execute(std::move(cmd1));
    cmd_history->execute(std::move(cmd2));
    cmd_history->undo();
    cmd_history->redo();
}

TEST_F(Given_a_command_history,
       Undoing_n_commands_after_executing_them_undoes_them_all)
{
    const auto n = urandom(10, 1000);
    std::vector<std::unique_ptr<mock_command>> mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique<mock_command>();

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

TEST_F(Given_a_command_history,
       Redoing_n_commands_after_undoing_them_redoes_them_all)
{
    const auto n = urandom(10, 1000);
    std::vector<std::unique_ptr<mock_command>> mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique<mock_command>();

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

TEST_F(Given_a_command_history,
       After_undoing_and_executing_once_redo_does_nothing)
{
    auto cmd = std::make_unique<mock_command>();
    auto cmd2 = std::make_unique<mock_command>();

    EXPECT_CALL(*cmd, redo).Times(0);
    EXPECT_CALL(*cmd2, redo).Times(0);

    cmd_history->execute(std::move(cmd));
    cmd_history->undo();
    cmd_history->execute(std::move(cmd2));
    cmd_history->redo();
}

TEST_F(Given_a_command_history,
       After_undoing_n_times_and_executing_once_redo_does_nothing)
{
    const auto n = urandom(2, 10);
    std::vector<std::unique_ptr<mock_command>> mock_commands(n);
    for (auto& cmd : mock_commands)
        cmd = std::make_unique<mock_command>();

    for (const auto& cmd : mock_commands)
    {
        EXPECT_CALL(*cmd, redo).Times(0);
    }

    for (auto& cmd : mock_commands)
        cmd_history->execute(std::move(cmd));

    for (auto i = 0; i < n; ++i)
        cmd_history->undo();

    auto cmd2 = std::make_unique<mock_command>();
    cmd_history->execute(std::move(cmd2));
    cmd_history->redo();
}

} // namespace