#include "utility/all.hpp"
#include "visualization/layout/services.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace visualization::layout;
using namespace visualization::command;
using namespace visualization::communication;
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
        s = std::make_unique<topology>(cube(10));
        l = std::make_unique<gursoy_atun_layout>(*g, *s);
    }

protected:
    std::unique_ptr<event_bus> bus;
    std::unique_ptr<graph> g;
    std::unique_ptr<command_history> cmds;
    std::unique_ptr<layout> l;
    std::unique_ptr<topology> s;
};

class An_update_layout_service
    : public Given_a_bus_graph_cmds_topology_and_layout
{
public:
    using mock_subscriber =
        NiceMock<MockFunction<event_bus::subscriber<layout_request_event>>>;

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
    auto* prev = &l;

    std::invoke(*service, layout_request_event("gursoy_atun", "gursoy_atun"));

    ASSERT_EQ(prev, &l);
}

TEST_F(An_update_layout_service,
       Wont_post_a_layout_response_when_requested_same_type)
{
    const auto event = layout_request_event("gursoy_atun", "gursoy_atun");
    mock_subscriber mock;
    bus->subscribe<layout_request_event>(mock.AsStdFunction());

    EXPECT_CALL(mock, Call(event)).Times(0);

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

} // namespace