#include "presentation/commands.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace presentation;

namespace
{

/***********************************************************
 * Weights                                                 *
 ***********************************************************/

class weights_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< weights_backend >(
            weights_config({ { "weak", 2 }, { "strong", 10 } }));
    }

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< weights_backend > backend;
};

TEST_F(weights_commands_tests, update_weight_updates_the_backend_accordingly)
{
    update_dependency_weight(*cmds, *backend, "weak", 10);

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 10);
}

TEST_F(weights_commands_tests, update_weight_can_be_undone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
}

TEST_F(weights_commands_tests, update_weight_can_be_undone_and_redone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 20);
}

TEST_F(weights_commands_tests, restore_weights_updates_the_backend_accordingly)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);

    restore_weights(*cmds, *backend);

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 10);
}

TEST_F(weights_commands_tests, restore_weights_can_be_undone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);
    restore_weights(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 20);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 100);
}

TEST_F(weights_commands_tests, restore_weights_can_be_undone_and_redone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);
    restore_weights(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 10);
}

} // namespace