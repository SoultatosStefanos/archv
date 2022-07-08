#include "mocks/all.hpp"
#include "utility/all.hpp"
#include "visualization/layout/layout_factory.hpp"
#include "visualization/layout/presenter.hpp"
#include "visualization/layout/topology_factory.hpp"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace utility;
using namespace architecture;
using namespace visualization::layout;
using namespace visualization::layout::mocks;

class Given_mocked_layout_presenter_dependencies : public testing::Test
{
public:
    using nice_mock_layout_view = testing::NiceMock<mock_layout_view>;
    using nice_mock_layout_service = testing::NiceMock<mock_layout_service>;
    using presenter =
        layout_presenter<nice_mock_layout_view, nice_mock_layout_service>;

    void SetUp() override
    {
        view = std::make_unique<nice_mock_layout_view>();
        service = std::make_unique<nice_mock_layout_service>();
    }

protected:
    std::unique_ptr<nice_mock_layout_view> view;
    std::unique_ptr<nice_mock_layout_service> service;
};

TEST_F(Given_mocked_layout_presenter_dependencies,
       A_new_presenter_subscribes_to_the_service_change_events)
{
    EXPECT_CALL(*service, add_layout_changed_listener(testing::_));

    presenter(graph(), *view, *service);
}

class Given_a_layout_presenter_with_mocked_dependencies
    : public Given_mocked_layout_presenter_dependencies
{
public:
    void SetUp() override
    {
        Given_mocked_layout_presenter_dependencies::SetUp();
        pres = std::make_unique<presenter>(g, *view, *service);
    }

protected:
    static constexpr auto vertices_num = 100;

    graph g{vertices_num};
    std::unique_ptr<presenter> pres;
};

TEST_F(Given_a_layout_presenter_with_mocked_dependencies,
       An_update_view_call_will_request_the_view_to_draw_each_vertex)
{
    auto&& l = layout_factory::make_layout(
        layout_factory::gursoy_atun_type,
        g,
        topology_factory::make_topology(topology_factory::cube_type, 23));

    EXPECT_CALL(*view,
                draw_vertex(testing::_, testing::_, testing::_, testing::_))
        .Times(vertices_num);

    pres->update_view(*l);
}

} // namespace