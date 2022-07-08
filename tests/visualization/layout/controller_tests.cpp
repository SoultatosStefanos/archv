#include "mocks/all.hpp"
#include "visualization/layout/controller.hpp"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace visualization::layout;
using namespace visualization::layout::mocks;

class Given_mocked_dependencies : public testing::Test
{
public:
    using nice_mock_layout_view = testing::NiceMock<mock_layout_view>;
    using nice_mock_layout_service = testing::NiceMock<mock_layout_service>;
    using controller =
        layout_controller<nice_mock_layout_view, nice_mock_layout_service>;

    void SetUp() override
    {
        view = std::make_unique<nice_mock_layout_view>();
        service = std::make_unique<nice_mock_layout_service>();
    }

protected:
    std::unique_ptr<nice_mock_layout_view> view;
    std::unique_ptr<nice_mock_layout_service> service;
};

TEST_F(Given_mocked_dependencies,
       A_new_controller_subscribes_to_the_view_input_events)
{
    EXPECT_CALL(*view, add_layout_input_listener(testing::_));
    EXPECT_CALL(*view, add_topology_input_listener(testing::_));

    controller(*view, *service);
}

class Given_a_layout_controller_with_mocked_dependencies
    : public Given_mocked_dependencies
{
public:
    void SetUp() override
    {
        Given_mocked_dependencies::SetUp();
        ctrl = std::make_unique<controller>(*view, *service);
    }

protected:
    std::unique_ptr<controller> ctrl;
};

TEST_F(Given_a_layout_controller_with_mocked_dependencies,
       The_update_layout_service_is_called_when_the_layout_is_selected)
{
    std::string layout_type;

    EXPECT_CALL(*service, update_layout(layout_type));

    ctrl->layout_selected(layout_type);
}

TEST_F(Given_a_layout_controller_with_mocked_dependencies,
       The_update_topology_service_is_called_when_the_topology_is_selected)
{
    std::string topology_type;
    double scale = 9;

    EXPECT_CALL(*service, update_topology(topology_type, scale));

    ctrl->topology_selected(topology_type, scale);
}

} // namespace