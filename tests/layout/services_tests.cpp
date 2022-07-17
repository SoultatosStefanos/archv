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

class Given_a_bus_graph_cmds_topology_and_layout : public Test
{
public:
    void SetUp() override
    {
        bus = std::make_unique<event_bus>();
        g = std::make_unique<graph>();
        cmds = std::make_unique<command_history>();
        s = std::make_unique<cube>(10);
        l = std::make_unique<gursoy_atun_layout>(*g, *s);
    }

protected:
    std::unique_ptr<event_bus> bus;
    std::unique_ptr<graph> g;
    std::unique_ptr<command_history> cmds;
    std::unique_ptr<layout::layout> l;
    std::unique_ptr<topology> s;
};

class An_update_layout_service
    : public Given_a_bus_graph_cmds_topology_and_layout
{
public:
    using mock_subscriber =
        NiceMock<MockFunction<event_bus::subscriber<layout_response_event>>>;

    void SetUp() override
    {
        Given_a_bus_graph_cmds_topology_and_layout::SetUp();
        service =
            std::make_unique<update_layout_service>(*bus, *cmds, *g, *s, l);
    }

protected:
    std::unique_ptr<update_layout_service> service;
};

TEST_F(An_update_layout_service,
       Wont_change_the_layout_when_requested_same_type)
{
    const auto* prev = l.get();

    std::invoke(*service, layout_request_event("gursoy_atun"));

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_layout_service,
       Wont_post_a_layout_response_when_requested_same_type)
{
    const auto event = layout_request_event("gursoy_atun");
    mock_subscriber mock;
    bus->subscribe<layout_response_event>(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    std::invoke(*service, event);
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

class An_update_topology_service
    : public Given_a_bus_graph_cmds_topology_and_layout
{
public:
    using mock_subscriber =
        NiceMock<MockFunction<event_bus::subscriber<layout_response_event>>>;

    void SetUp() override
    {
        Given_a_bus_graph_cmds_topology_and_layout::SetUp();
        service =
            std::make_unique<update_topology_service>(*bus, *cmds, *g, s, l);
    }

protected:
    std::unique_ptr<update_topology_service> service;
};

TEST_F(An_update_topology_service,
       Wont_change_the_layout_when_requested_same_type_and_scale)
{
    const auto* prev = l.get();

    std::invoke(*service, topology_request_event{.type = "cube", .scale = 10});

    ASSERT_EQ(prev, l.get());
}

TEST_F(An_update_topology_service,
       Wont_post_a_layout_response_when_requested_same_type_and_scale)
{
    const auto event = topology_request_event{.type = "cube", .scale = 10};
    mock_subscriber mock;
    bus->subscribe<layout_response_event>(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    std::invoke(*service, event);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_and_topology_when_requested_different_type)
{
    assert(s->desc() == topology_traits<cube>::desc());

    const auto event = topology_request_event{.type = "sphere", .scale = 80};

    const auto* prev = l.get();

    std::invoke(*service, event);

    ASSERT_NE(prev, l.get());
    ASSERT_EQ(s->desc(), topology_traits<sphere>::desc());
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_type)
{
    const auto event = topology_request_event{.type = "cube", .scale = 80};
    mock_subscriber mock;
    bus->subscribe<layout_response_event>(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    std::invoke(*service, event);
}

TEST_F(An_update_topology_service,
       Will_change_the_layout_when_requested_different_scale)
{
    const auto event = topology_request_event{.type = "sphere", .scale = 100};
    const auto* prev = l.get();

    std::invoke(*service, event);

    ASSERT_NE(prev, l.get());
}

TEST_F(An_update_topology_service,
       Will_post_a_layout_response_when_requested_different_scale)
{
    const auto event = topology_request_event{.type = "sphere", .scale = 50};
    mock_subscriber mock;
    bus->subscribe<layout_response_event>(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    std::invoke(*service, event);
}

TEST_F(An_update_topology_service,
       Will_revert_to_initial_topology_after_requested_different_type_and_undo)
{
    assert(s->desc() == topology_traits<cube>::desc());

    const auto event = topology_request_event{.type = "sphere", .scale = 80};

    std::invoke(*service, event);

    EXPECT_EQ(s->desc(), topology_traits<sphere>::desc());

    cmds->undo();

    ASSERT_EQ(s->desc(), topology_traits<cube>::desc());
}

TEST_F(
    An_update_topology_service,
    Will_revert_to_changed_topology_after_requested_different_type_and_undo_and_redo)
{
    assert(s->desc() == topology_traits<cube>::desc());

    const auto event = topology_request_event{.type = "sphere", .scale = 80};

    std::invoke(*service, event);

    EXPECT_EQ(s->desc(), topology_traits<sphere>::desc());

    cmds->undo();

    EXPECT_EQ(s->desc(), topology_traits<cube>::desc());

    cmds->redo();

    ASSERT_EQ(s->desc(), topology_traits<sphere>::desc());
}

} // namespace