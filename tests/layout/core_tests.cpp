#include "layout/core.hpp"
#include "undo_redo/all.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace lay = layout;

using namespace testing;
using namespace layout;
using namespace undo_redo;

namespace
{

class a_layout_core : public Test
{
protected:
    using graph = boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::directedS,
        boost::no_property,
        boost::no_property >;

    using weight_function = std::function< int(graph::edge_descriptor) >;
    using weight_map = boost::
        function_property_map< weight_function, graph::edge_descriptor, int >;

    using mock_layout_slot
        = NiceMock< MockFunction< void(const lay::layout< graph >&) > >;

    using mock_topology_slot
        = NiceMock< MockFunction< void(const topology&) > >;

    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        g = std::make_unique< graph >();

        sys = std::make_unique< core< graph, weight_map > >(
            *cmds,
            *g,
            weight_map([](auto) { return 1; }),
            initial_layout_type,
            initial_topology_type,
            initial_topology_scale);
    }

    static constexpr auto initial_layout_type = "Gursoy Atun";
    static constexpr auto initial_topology_type = "Sphere";
    static constexpr auto initial_topology_scale = 10;

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< graph > g;

    std::unique_ptr< core< graph, weight_map > > sys;
};

TEST_F(
    a_layout_core, wont_emit_the_layout_when_updating_layout_with_the_same_type)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    sys->update_layout(initial_layout_type);
}

// TODO
TEST_F(
    a_layout_core,
    will_emit_the_layout_when_updating_layout_with_different_type)
{
    SUCCEED();
}

// TODO
TEST_F(
    a_layout_core, will_revert_to_initial_layout_after_updating_layout_and_undo)
{
    SUCCEED();
}

// TODO
TEST_F(
    a_layout_core,
    will_revert_to_changed_layout_after_updating_layout_and_undo_and_redo)
{
    SUCCEED();
}

TEST_F(
    a_layout_core,
    wont_emit_the_topology_when_updating_topology_with_the_same_type_and_scale)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    sys->update_topology(initial_topology_type, initial_topology_scale);
}

TEST_F(
    a_layout_core,
    will_emit_the_topology_when_updating_topology_with_different_type)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());
    constexpr auto new_topology_type = "Cube";

    static_assert(new_topology_type != initial_topology_type);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->update_topology(new_topology_type, initial_topology_scale);
}

TEST_F(
    a_layout_core,
    will_emit_the_layout_when_updating_topology_with_different_type)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());
    constexpr auto new_topology_type = "Cube";

    static_assert(new_topology_type != initial_topology_type);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->update_topology(new_topology_type, initial_topology_scale);
}

TEST_F(
    a_layout_core,
    will_emit_the_topology_when_updating_topology_with_different_scale)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());
    constexpr auto new_topology_scale = 80;

    static_assert(new_topology_scale != initial_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->update_topology(initial_topology_type, new_topology_scale);
}

TEST_F(
    a_layout_core,
    will_emit_the_layout_when_updating_topology_with_different_scale)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());
    constexpr auto new_topology_scale = 80;

    static_assert(new_topology_scale != initial_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->update_topology(initial_topology_type, new_topology_scale);
}

TEST_F(
    a_layout_core,
    will_revert_to_initial_topology_after_updating_topology_and_undo)
{
    constexpr auto new_topology_type = "Cube";

    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_EQ(sys->get_topology().desc(), new_topology_type);

    cmds->undo();

    ASSERT_EQ(sys->get_topology().desc(), initial_topology_type);
}

// TODO
TEST_F(
    a_layout_core,
    will_revert_to_initial_layout_after_updating_topology_and_undo)
{
    SUCCEED();
}

TEST_F(
    a_layout_core,
    will_revert_to_changed_topology_after_updating_topology_and_undo_and_redo)
{
    constexpr auto new_topology_type = "Cube";

    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_EQ(sys->get_topology().desc(), new_topology_type);

    cmds->undo();

    EXPECT_EQ(sys->get_topology().desc(), initial_topology_type);

    cmds->redo();

    ASSERT_EQ(sys->get_topology().desc(), new_topology_type);
}

// TODO
TEST_F(
    a_layout_core,
    will_revert_to_changed_layout_after_updating_topology_and_undo_and_redo)
{
    SUCCEED();
}

TEST_F(
    a_layout_core,
    wont_emit_the_topology_when_reverting_from_initial_properties)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    sys->revert_to_defaults();
}

TEST_F(
    a_layout_core, wont_emit_the_layout_when_reverting_from_initial_properties)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    sys->revert_to_defaults();
}

// TODO
TEST_F(
    a_layout_core,
    will_emit_the_topology_when_reverting_from_different_layout_type)
{
    SUCCEED();
}

// TODO
TEST_F(
    a_layout_core,
    will_emit_the_layout_when_reverting_from_different_layout_type)
{
    SUCCEED();
}

TEST_F(
    a_layout_core,
    will_emit_the_topology_when_reverting_from_different_topology_type)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());

    constexpr auto new_topology_type = "Cube";
    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->revert_to_defaults();
}

TEST_F(
    a_layout_core,
    will_emit_the_layout_when_reverting_from_different_topology_type)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());

    constexpr auto new_topology_type = "Cube";
    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->revert_to_defaults();
}

TEST_F(
    a_layout_core,
    will_emit_the_topology_when_reverting_from_different_topology_scale)
{
    mock_topology_slot mock;
    sys->connect_to_topology(mock.AsStdFunction());

    constexpr auto new_topology_scale = 2;
    static_assert(new_topology_scale != initial_topology_scale);

    sys->update_topology(initial_topology_type, new_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->revert_to_defaults();
}

TEST_F(
    a_layout_core,
    will_emit_the_layout_when_reverting_from_different_topology_scale)
{
    mock_layout_slot mock;
    sys->connect_to_layout(mock.AsStdFunction());

    constexpr auto new_topology_scale = 2;
    static_assert(new_topology_scale != initial_topology_scale);

    sys->update_topology(initial_topology_type, new_topology_scale);

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    sys->revert_to_defaults();
}

TEST_F(a_layout_core, will_revert_to_changed_topology_after_reverting_and_undo)
{
    constexpr auto new_topology_type = "Cube";
    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_EQ(sys->get_topology().desc(), new_topology_type);

    sys->revert_to_defaults();

    EXPECT_EQ(sys->get_topology().desc(), initial_topology_type);

    cmds->undo();

    ASSERT_EQ(sys->get_topology().desc(), new_topology_type);
}

// TODO
TEST_F(a_layout_core, will_revert_to_changed_layout_after_reverting_and_undo)
{
    SUCCEED();
}

TEST_F(
    a_layout_core,
    will_revert_to_initial_topology_after_reverting_and_undo_and_redo)
{
    constexpr auto new_topology_type = "Cube";
    static_assert(new_topology_type != initial_topology_type);

    sys->update_topology(new_topology_type, initial_topology_scale);

    EXPECT_EQ(sys->get_topology().desc(), new_topology_type);

    sys->revert_to_defaults();

    EXPECT_EQ(sys->get_topology().desc(), initial_topology_type);

    cmds->undo();

    EXPECT_EQ(sys->get_topology().desc(), new_topology_type);

    cmds->redo();

    ASSERT_EQ(sys->get_topology().desc(), initial_topology_type);
}

// TODO
TEST_F(
    a_layout_core,
    will_revert_to_initial_layout_after_reverting_and_undo_and_redo)
{
    SUCCEED();
}

TEST_F(a_layout_core, will_abort_when_updating_with_invalid_layout_type)
{
    EXPECT_DEATH(sys->update_layout("aaaaaaaa"), "");
}

TEST_F(a_layout_core, will_abort_when_updating_with_invalid_topology_type)
{
    EXPECT_DEATH(sys->update_topology("aaaaaaaa", 22), "");
}

} // namespace