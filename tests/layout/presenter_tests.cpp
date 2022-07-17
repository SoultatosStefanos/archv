#include "layout/presenter.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace architecture;
using namespace utility;
using namespace layout;

class view_interface
{
public:
    view_interface() = default;
    view_interface(const view_interface&) = default;
    view_interface(view_interface&&) = default;
    virtual ~view_interface() = default;

    auto operator=(const view_interface&) -> view_interface& = default;
    auto operator=(view_interface&&) -> view_interface& = default;

    virtual void draw_vertex(const std::string&, double, double, double) = 0;
};

class view_mock : public view_interface
{
public:
    view_mock() = default;
    ~view_mock() override = default;

    MOCK_METHOD(void,
                draw_vertex,
                (const std::string&, double, double, double),
                (override));
};

class A_layout_presenter : public testing::Test
{
public:
    using present = presenter<view_interface>;

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        pres = std::make_unique<present>(*pipeline, g, mock);
    }

protected:
    static constexpr auto vertices_num = 100;

    view_mock mock;
    graph g{vertices_num};

    std::unique_ptr<event_bus> pipeline;
    std::unique_ptr<present> pres;
};

TEST_F(
    A_layout_presenter,
    Calls_the_view_when_a_layout_changed_event_is_posted_to_its_pipeline_vertices_num_times)
{
    auto s = cube(3);
    auto l = std::make_unique<gursoy_atun_layout>(g, s);
    layout_response_event event{.curr = *l};

    EXPECT_CALL(mock,
                draw_vertex(testing::_, testing::_, testing::_, testing::_))
        .Times(vertices_num);

    pipeline->post(event);
}

TEST_F(
    A_layout_presenter,
    Calls_the_view_when_a_layout_changed_event_is_posted_n_times_to_its_pipeline_vertices_num_times_n_times)
{
    auto s = cube(3);
    auto l = std::make_unique<gursoy_atun_layout>(g, s);
    layout_response_event event{.curr = *l};
    const auto n = urandom(1, 5);

    EXPECT_CALL(mock,
                draw_vertex(testing::_, testing::_, testing::_, testing::_))
        .Times(vertices_num * n);

    for (auto i = 0; i < n; ++i)
        pipeline->post(event);
}

} // namespace