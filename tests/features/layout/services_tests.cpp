#include "dependencies/all.hpp"
#include "features/layout/services.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace dependencies;
using namespace features::layout;
using namespace utility;
using namespace testing;

namespace lay = features::layout;

class Given_a_graph_cmds_topology_and_layout : public Test
{
public:
    void SetUp() override
    {
        g = std::make_unique<graph>();
        cmds = std::make_unique<command_history>();
        s = topology_factory::make_topology(topology_factory::cube_desc, 10);
        l = layout_factory::make_layout(
            layout_factory::gursoy_atun_desc, *g, *s);
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
    using mock_subscriber =
        NiceMock<MockFunction<void(const lay::layout&, const topology&)>>;

    void SetUp() override
    {
        Given_a_graph_cmds_topology_and_layout::SetUp();
        service = std::make_unique<update_layout_service>(*cmds, *g, l, s);
    }

protected:
    std::unique_ptr<update_layout_service> service;
};

TEST_F(An_update_layout_service,
       Wont_change_the_layout_when_requested_same_desc)
{
    const auto* prev = l.get();

    service->execute(l->desc());

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_layout_service,
       Wont_post_a_layout_response_when_requested_same_desc)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(0);

    service->execute(l->desc());
}

// TODO
TEST_F(An_update_layout_service,
       Will_change_the_layout_when_requested_different_desc)
{
    SUCCEED();
}

// TODO
TEST_F(An_update_layout_service,
       Will_post_a_layout_response_when_requested_different_desc)
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
        service = std::make_unique<update_topology_service>(*cmds, *g, l, s);
    }

protected:
    std::unique_ptr<update_topology_service> service;
};

TEST_F(An_update_topology_service,
       Wont_change_the_layout_when_requested_same_desc_and_scale)
{
    const auto* prev = l.get();

    service->execute(s->desc(), s->scale());

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_topology_service,
       Wont_post_a_layout_response_when_requested_same_desc_and_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(0);

    service->execute(s->desc(), s->scale());
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_and_topology_when_requested_different_desc)
{
    const auto* prev = l.get();
    const auto& prev_id = typeid(*s);

    assert(s->desc() != topology_factory::sphere_desc);

    service->execute(topology_factory::sphere_desc, 80);

    ASSERT_NE(prev, l.get());
    ASSERT_NE(typeid(*s), prev_id);
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_desc)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    assert(s->desc() != topology_factory::sphere_desc);

    service->execute(topology_factory::sphere_desc, 80);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_when_requested_different_scale)
{
    const auto* prev = l.get();

    service->execute(s->desc(), 100);

    ASSERT_NE(prev, l.get());
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    service->execute(s->desc(), 50);
}

TEST_F(An_update_topology_service,
       Will_revert_to_initial_topology_after_requested_different_desc_and_undo)
{
    assert(s->desc() != topology_factory::sphere_desc);

    service->execute(topology_factory::sphere_desc, 80);

    EXPECT_EQ(s->desc(), topology_factory::sphere_desc);
    EXPECT_EQ(s->scale(), 80);

    cmds->undo();

    ASSERT_EQ(s->desc(), topology_factory::cube_desc);
    ASSERT_EQ(s->scale(), 10);
}

TEST_F(
    An_update_topology_service,
    Will_revert_to_changed_topology_after_requested_different_desc_and_undo_and_redo)
{
    assert(s->desc() != topology_factory::sphere_desc);

    service->execute(topology_factory::sphere_desc, 80);

    EXPECT_EQ(s->desc(), topology_factory::sphere_desc);
    EXPECT_EQ(s->scale(), 80);

    cmds->undo();

    EXPECT_EQ(s->desc(), topology_factory::cube_desc);
    EXPECT_EQ(s->scale(), 10);

    cmds->redo();

    ASSERT_EQ(s->desc(), topology_factory::sphere_desc);
    ASSERT_EQ(s->scale(), 80);
}

} // namespace