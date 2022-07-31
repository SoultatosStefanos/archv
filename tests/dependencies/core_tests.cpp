#include "dependencies/core.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace dependencies;
using namespace utility;

namespace
{

class a_dependencies_core : public Test
{
protected:
    using mock_slot = NiceMock<
        MockFunction<void(const core::dependency_type&, core::weight)>>;

    void SetUp() override
    {
        cmds = std::make_unique<command_history>();
        sys = std::make_unique<core>(*cmds, table);
    }

    static constexpr auto set_dependency = "inheritance";
    static constexpr auto set_weight = 100;

    const core::hash_table table{{set_dependency, set_weight}};
    std::unique_ptr<command_history> cmds;

    std::unique_ptr<core> sys;
};

TEST_F(a_dependencies_core,
       wont_emit_dependency_and_weight_when_setting_same_value)
{
    mock_slot mock;
    sys->connect(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(set_dependency, set_weight)).Times(0);

    sys->set_weight(set_dependency, set_weight);
}

TEST_F(a_dependencies_core,
       will_emit_dependency_and_weight_when_setting_different_value)
{
    mock_slot mock;
    sys->connect(mock.AsStdFunction());

    constexpr auto new_weight = 200;

    static_assert(new_weight != set_weight);

    EXPECT_CALL(mock, Call(set_dependency, new_weight)).Times(1);

    sys->set_weight(set_dependency, new_weight);
}

TEST_F(a_dependencies_core,
       will_return_updated_repo_after_successfully_setting_and_querying)
{
    constexpr auto new_weight = 200;

    static_assert(new_weight != set_weight);

    sys->set_weight(set_dependency, new_weight);

    ASSERT_EQ(sys->get_repo().get_weight(set_dependency), new_weight);
}

TEST_F(a_dependencies_core, will_revert_to_set_value_after_setting_and_undo)
{
    constexpr auto new_weight = 200;
    static_assert(new_weight != set_weight);

    sys->set_weight(set_dependency, new_weight);

    EXPECT_EQ(sys->get_repo().get_weight(set_dependency), new_weight);

    cmds->undo();

    ASSERT_EQ(sys->get_repo().get_weight(set_dependency), set_weight);
}

TEST_F(a_dependencies_core,
       will_revert_to_changed_value_after_setting_and_undo_and_redo)
{
    constexpr auto new_weight = 200;
    static_assert(new_weight != set_weight);

    sys->set_weight(set_dependency, new_weight);

    EXPECT_EQ(sys->get_repo().get_weight(set_dependency), new_weight);

    cmds->undo();

    EXPECT_EQ(sys->get_repo().get_weight(set_dependency), set_weight);

    cmds->redo();

    ASSERT_EQ(sys->get_repo().get_weight(set_dependency), new_weight);
}

} // namespace