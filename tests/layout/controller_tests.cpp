#include "layout/controller.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace utility;
using namespace layout;

class A_layout_controller : public testing::Test
{
public:
    using update_layout_func = std::function<void(const layout_request_event&)>;
    using update_layout_mock =
        testing::NiceMock<testing::MockFunction<update_layout_func>>;

    using update_top_func = std::function<void(const topology_request_event&)>;
    using update_topology_mock =
        testing::NiceMock<testing::MockFunction<update_top_func>>;

    using control = controller<update_layout_func, update_top_func>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        ctrl = std::make_unique<control>(
            *pipeline, ul_mock.AsStdFunction(), ut_mock.AsStdFunction());
    }

protected:
    update_layout_mock ul_mock;
    update_topology_mock ut_mock;

    std::unique_ptr<event_bus> pipeline;
    std::unique_ptr<control> ctrl;
};

TEST_F(
    A_layout_controller,
    Calls_the_update_layout_service_when_a_layout_request_event_is_posted_to_its_pipeline)
{
    layout_request_event event{.old_type = "random", .new_type = "gursoy atun"};

    EXPECT_CALL(ul_mock, Call(event)).Times(1);

    pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_layout_service_n_times_when_a_layout_request_event_is_posted_to_its_pipeline_n_times)
{
    layout_request_event event{.old_type = "random", .new_type = "gursoy atun"};
    const auto n = urandom(0, 1000);

    EXPECT_CALL(ul_mock, Call(event)).Times(n);

    for (auto i = 0; i < n; ++i)
        pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_topology_service_when_a_topology_request_event_is_posted_to_its_pipeline)
{
    topology_request_event event{.layout_type = "gursoy atun",
                                 .old_type = "sphere",
                                 .old_scale = 20,
                                 .new_type = "cube",
                                 .new_scale = 30};

    EXPECT_CALL(ut_mock, Call(event)).Times(1);

    pipeline->post(event);
}

TEST_F(
    A_layout_controller,
    Calls_the_update_topology_service_n_times_when_a_topology_input_event_is_posted_to_its_pipeline_n_times)
{
    topology_request_event event{.layout_type = "gursoy atun",
                                 .old_type = "sphere",
                                 .old_scale = 20,
                                 .new_type = "cube",
                                 .new_scale = 30};

    const auto n = urandom(0, 1000);

    EXPECT_CALL(ut_mock, Call(event)).Times(n);

    for (auto i = 0; i < n; ++i)
        pipeline->post(event);
}

} // namespace