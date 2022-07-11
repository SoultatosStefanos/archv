#include "utility/all.hpp"
#include "visualization/communication/all.hpp"
#include "visualization/layout/layout_factory.hpp"
#include "visualization/layout/services.hpp"
#include "visualization/layout/topology_factory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace architecture;
using namespace visualization::communication;
using namespace visualization::layout;
using namespace utility;

class An_update_layout_service : public testing::Test
{
public:
    using mock_subscriber = testing::NiceMock<
        testing::MockFunction<event_bus::subscriber<layout_response_event>>>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        l = layout_factory::make_layout(layout_type, g, space);

        update_layout = std::make_unique<update_layout_service>(
            *pipeline, g, l, layout_type, &space);

        pipeline->subscribe<layout_response_event>(mock.AsStdFunction());
    }

protected:
    std::unique_ptr<event_bus> pipeline;
    std::unique_ptr<layout> l;
    std::string layout_type = layout_factory::gursoy_atun_type;
    graph g;
    topology space = cube(38);

    std::unique_ptr<update_layout_service> update_layout;

    mock_subscriber mock;
};

TEST_F(An_update_layout_service,
       Will_not_change_the_layout_if_given_type_is_same_as_curr)
{
    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    update_layout->update(layout_type);
}

// TODO Implement later, after adding more layout types.
TEST_F(An_update_layout_service,
       Will_change_the_layout_if_given_type_is_different_to_curr)
{
    SUCCEED();
}

TEST_F(
    An_update_layout_service,
    Will_change_the_layout_when_a_topology_changed_event_is_posted_to_its_pipeline)
{
    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    pipeline->post(topology_response_event{.curr = space});
}

class An_update_topology_service : public testing::Test
{
public:
    using mock_subscriber = testing::NiceMock<
        testing::MockFunction<event_bus::subscriber<topology_response_event>>>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        space = topology_factory::make_topology(topology_type, topology_scale);

        update_topology = std::make_unique<update_topology_service>(
            *pipeline, g, space, topology_type, topology_scale);

        pipeline->subscribe<topology_response_event>(mock.AsStdFunction());
    }

protected:
    std::unique_ptr<event_bus> pipeline;
    graph g;

    std::string topology_type = topology_factory::sphere_type;
    double topology_scale = 38;
    topology space;

    std::unique_ptr<update_topology_service> update_topology;

    mock_subscriber mock;
};

TEST_F(An_update_topology_service,
       Will_not_change_the_topology_if_given_type_and_scale_are_same_as_curr)
{
    EXPECT_CALL(mock, Call(testing::_)).Times(0);

    update_topology->update(topology_type, topology_scale);
}

TEST_F(An_update_topology_service,
       Will_change_the_topology_if_given_type_is_different_to_curr)
{
    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    update_topology->update(topology_factory::cube_type, topology_scale);
}

TEST_F(An_update_topology_service,
       Will_change_the_topology_if_given_scale_is_different_to_curr)
{
    EXPECT_CALL(mock, Call(testing::_)).Times(1);

    update_topology->update(topology_type, 100);
}

} // namespace