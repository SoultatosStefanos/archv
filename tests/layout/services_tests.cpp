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
        s = topology_factory::make_topology(topology_factory::type_name::cube,
                                            10);
        l = layout_factory::make_layout(
            layout_factory::type_name::gursoy_atun, *g, *s);
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
       Wont_change_the_layout_when_requested_same_type)
{
    const auto* prev = l.get();

    std::invoke(*service, layout_factory::resolve_type(*l));

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_layout_service,
       Wont_post_a_layout_response_when_requested_same_type)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(0);

    std::invoke(*service, layout_factory::resolve_type(*l));
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
        service = std::make_unique<update_topology_service>(*cmds, *g, l, s);
    }

protected:
    std::unique_ptr<update_topology_service> service;
};

TEST_F(An_update_topology_service,
       Wont_change_the_layout_when_requested_same_type_and_scale)
{
    const auto* prev = l.get();

    std::invoke(*service, topology_factory::resolve_type(*s), s->scale());

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_topology_service,
       Wont_post_a_layout_response_when_requested_same_type_and_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(0);

    std::invoke(*service, topology_factory::resolve_type(*s), s->scale());
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_and_topology_when_requested_different_type)
{
    const auto* prev = l.get();
    const auto& prev_id = typeid(*s);

    assert(topology_factory::resolve_type(*s) !=
           topology_factory::type_name::sphere);

    std::invoke(*service, topology_factory::type_name::sphere, 80);

    ASSERT_NE(prev, l.get());
    ASSERT_NE(typeid(*s), prev_id);
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_type)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    assert(topology_factory::resolve_type(*s) !=
           topology_factory::type_name::sphere);

    std::invoke(*service, topology_factory::type_name::sphere, 80);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_when_requested_different_scale)
{
    const auto* prev = l.get();

    std::invoke(*service, topology_factory::resolve_type(*s), 100);

    ASSERT_NE(prev, l.get());
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_scale)
{
    mock_subscriber mock;
    service->on_layout_response(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_, testing::_)).Times(1);

    std::invoke(*service, topology_factory::resolve_type(*s), 50);
}

TEST_F(An_update_topology_service,
       Will_revert_to_initial_topology_after_requested_different_type_and_undo)
{
    assert(topology_factory::resolve_type(*s) !=
           topology_factory::type_name::sphere);

    std::invoke(*service, topology_factory::type_name::sphere, 80);

    EXPECT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::sphere);
    EXPECT_EQ(s->scale(), 80);

    cmds->undo();

    ASSERT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::cube);
    ASSERT_EQ(s->scale(), 10);
}

TEST_F(
    An_update_topology_service,
    Will_revert_to_changed_topology_after_requested_different_type_and_undo_and_redo)
{
    assert(topology_factory::resolve_type(*s) !=
           topology_factory::type_name::sphere);

    std::invoke(*service, topology_factory::type_name::sphere, 80);

    EXPECT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::sphere);
    EXPECT_EQ(s->scale(), 80);

    cmds->undo();

    EXPECT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::cube);
    EXPECT_EQ(s->scale(), 10);

    cmds->redo();

    ASSERT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::sphere);
    ASSERT_EQ(s->scale(), 80);
}

} // namespace