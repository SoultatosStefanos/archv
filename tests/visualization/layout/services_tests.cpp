#include "utility/all.hpp"
#include "visualization/communication/all.hpp"
#include "visualization/layout/layout_factory.hpp"
#include "visualization/layout/services.hpp"

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
        testing::MockFunction<event_bus::subscriber<layout_changed_event>>>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        l = layout_factory::make_layout(layout_type, g, space);

        update_layout = std::make_unique<update_layout_service>(
            *pipeline, g, l, layout_type, &space);

        pipeline->subscribe<layout_changed_event>(mock.AsStdFunction());
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

    pipeline->post(topology_changed_event{.curr = space});
}

} // namespace