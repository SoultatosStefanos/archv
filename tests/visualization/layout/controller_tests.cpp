#include "utility/all.hpp"
#include "visualization/layout/controller.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace utility;
using namespace visualization::layout;
using namespace visualization::communication;

class A_layout_controller : public testing::Test
{
public:
    using update_layout_mock = testing::NiceMock<
        testing::MockFunction<controller::update_layout_service>>;

    using update_topology_mock = testing::NiceMock<
        testing::MockFunction<controller::update_topology_service>>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        ctrl = std::make_unique<controller>(
            *pipeline, ul_mock.AsStdFunction(), ut_mock.AsStdFunction());
    }

protected:
    update_layout_mock ul_mock;
    update_topology_mock ut_mock;

    std::unique_ptr<event_bus> pipeline;
    std::unique_ptr<controller> ctrl;
};

TEST_F(
    A_layout_controller,
    Calls_the_update_layout_service_when_a_layout_input_event_is_posted_to_its_pipeline)
{
    layout_input_event event{.type = "aaa"};

    EXPECT_CALL(ul_mock, Call(event.type)).Times(1);

    pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_layout_service_n_times_when_a_layout_input_event_is_posted_to_its_pipeline_n_times)
{
    layout_input_event event{.type = "aaa"};
    const auto n = urandom(0, 1000);

    EXPECT_CALL(ul_mock, Call(event.type)).Times(n);

    for (auto i = 0; i < n; ++i)
        pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_topology_service_when_a_topology_input_event_is_posted_to_its_pipeline)
{
    topology_input_event event{.type = "bbb", .scale = 29};

    EXPECT_CALL(ut_mock, Call(event.type, event.scale)).Times(1);

    pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_topology_service_n_times_when_a_topology_input_event_is_posted_to_its_pipeline_n_times)
{
    topology_input_event event{.type = "bbb", .scale = 29};
    const auto n = urandom(0, 1000);

    EXPECT_CALL(ut_mock, Call(event.type, event.scale)).Times(n);

    for (auto i = 0; i < n; ++i)
        pipeline->post(event);
}

} // namespace