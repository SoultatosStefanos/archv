#include "architecture/all.hpp"
#include "layout/services.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace layout;
using namespace utility;
using namespace testing;

namespace lay = layout;

class Given_a_graph_cmds_topology_and_layout : public Test
{
public:
    void SetUp() override
    {
        g = std::make_unique<graph>();
        cmds = std::make_unique<command_history>();
        s = topology_factory::make_topology(typeid(cube).name(), 10);
        l = layout_factory::make_layout(
            typeid(gursoy_atun_layout).name(), *g, *s);
    }

protected:
    std::unique_ptr<graph> g;
    std::unique_ptr<command_history> cmds;
    layout_factory::pointer l;
    topology_factory::pointer s;
};

class An_update_layout_service : public Given_a_graph_cmds_topology_and_layout
{
public:
    using mock_subscriber = NiceMock<MockFunction<void(const lay::layout&)>>;

    void SetUp() override
    {
        Given_a_graph_cmds_topology_and_layout::SetUp();
        service = std::make_unique<update_layout_service>(*cmds);
    }

protected:
    std::unique_ptr<update_layout_service> service;
};

TEST_F(An_update_layout_service,
       Wont_change_the_layout_when_requested_same_type)
{
    const auto* prev = l.get();

    std::invoke(*service, typeid(gursoy_atun_layout).name(), *g, l, s);

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_layout_service,
       Wont_post_a_layout_response_when_requested_same_type)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    std::invoke(*service, typeid(gursoy_atun_layout).name(), *g, l, s);
}

// TODO
TEST_F(An_update_layout_service,
       Will_change_the_layout_when_requested_different_type)
{
    SUCCEED();
}

// TODO
TEST_F(An_update_layout_service,
       Will_post_a_layout_response_when_requested_different_type)
{
    SUCCEED();
}

// TODO
TEST_F(An_update_layout_service,
       Will_revert_to_initial_layout_after_execute_and_undo)
{
    SUCCEED();
}

TEST_F(An_update_layout_service,
       Will_revert_to_changed_layout_after_execute_undo_and_redo)
{
    SUCCEED();
}

class An_update_topology_service : public Given_a_graph_cmds_topology_and_layout
{
public:
    using mock_subscriber =
        NiceMock<MockFunction<void(const lay::layout&, const topology&)>>;

    void SetUp() override
    {
        Given_a_graph_cmds_topology_and_layout::SetUp();
        service = std::make_unique<update_topology_service>(*cmds);
    }

protected:
    std::unique_ptr<update_topology_service> service;
};

TEST_F(An_update_topology_service,
       Wont_change_the_layout_when_requested_same_type_and_scale)
{
    const auto* prev = l.get();

    std::invoke(*service, typeid(*s).name(), s->scale(), *g, l, s);

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_topology_service,
       Wont_post_a_layout_response_when_requested_same_type_and_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(0);

    std::invoke(*service, typeid(*s).name(), s->scale(), *g, l, s);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_and_topology_when_requested_different_type)
{
    const auto* prev = l.get();

    std::invoke(*service, typeid(sphere).name(), 80, *g, l, s);

    ASSERT_NE(prev, l.get());
    ASSERT_EQ(typeid(*s), typeid(sphere));
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_type)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    std::invoke(*service, typeid(cube).name(), 80, *g, l, s);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_when_requested_different_scale)
{
    const auto* prev = l.get();

    std::invoke(*service, typeid(sphere).name(), 100, *g, l, s);

    ASSERT_NE(prev, l.get());
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    std::invoke(*service, typeid(sphere).name(), 50, *g, l, s);
}

TEST_F(An_update_topology_service,
       Will_revert_to_initial_topology_after_requested_different_type_and_undo)
{
    assert(typeid(*s) == typeid(cube));

    std::invoke(*service, typeid(sphere).name(), 80, *g, l, s);

    EXPECT_EQ(typeid(*s), typeid(sphere));

    cmds->undo();

    ASSERT_EQ(typeid(*s), typeid(cube));
}

TEST_F(
    An_update_topology_service,
    Will_revert_to_changed_topology_after_requested_different_type_and_undo_and_redo)
{
    assert(typeid(*s) == typeid(cube));

    std::invoke(*service, typeid(sphere).name(), 80, *g, l, s);

    EXPECT_EQ(typeid(*s), typeid(sphere));

    cmds->undo();

    EXPECT_EQ(typeid(*s), typeid(cube));

    cmds->redo();

    ASSERT_EQ(typeid(*s), typeid(sphere));
}

} // namespace