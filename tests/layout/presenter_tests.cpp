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

namespace lay = layout;

struct dummy_view
{
    int draw_vertex_called_times = 0;
    void draw_vertex(const std::string&, double, double, double)
    {
        ++draw_vertex_called_times;
    }

    std::string selected_layout;
    void update_layout_selection(std::string l)
    {
        selected_layout = std::move(l);
    }

    std::string selected_topology;
    double selected_topology_scale;
    void update_topology_selection(std::string space, double scale)
    {
        selected_topology = std::move(space);
        selected_topology_scale = scale;
    }

    void on_layout_request(const std::function<void(const std::string&)>&) {}

    void
    on_topology_request(const std::function<void(const std::string&, double)>&)
    {}
};

struct dummy_update_layout
{
    void on_layout_response(
        const std::function<void(const lay::layout&, const topology&)>&)
    {}

    bool called = false;
    layout_factory::descriptor type;
    void operator()(layout_factory::descriptor t)
    {
        called = true;
        type = std::move(t);
    }
};

struct dummy_update_topology
{
    void on_layout_response(
        const std::function<void(const lay::layout&, const topology&)>&)
    {}

    bool called = false;
    topology_factory::descriptor type;
    topology_factory::scale_type scale;
    void operator()(topology_factory::descriptor t,
                    topology_factory::scale_type s)
    {
        called = true;
        type = std::move(t);
        scale = s;
    }
};

class A_layout_presenter : public testing::Test
{
public:
    using test_presenter =
        presenter<dummy_view, dummy_update_layout, dummy_update_topology>;

    void SetUp() override { pres = std::make_unique<test_presenter>(g); }

protected:
    static constexpr auto vertices_num = 100;

    graph g{vertices_num};
    std::unique_ptr<test_presenter> pres;
};

TEST_F(A_layout_presenter,
       Given_a_layout_and_topology_draws_each_vertex_on_view)
{
    const auto space = cube(10);
    const auto lay = gursoy_atun_layout(g, space);

    pres->update_view(lay, space);

    ASSERT_EQ(pres->get_view().draw_vertex_called_times, vertices_num);
}

TEST_F(A_layout_presenter,
       Given_a_layout_and_topology_sets_the_formatted_selected_elements_at_view)
{
    const auto space = cube(10);
    const auto lay = gursoy_atun_layout(g, space);

    pres->update_view(lay, space);

    ASSERT_EQ(pres->get_view().selected_layout, lay.desc());
    ASSERT_EQ(pres->get_view().selected_topology, space.desc());
    ASSERT_EQ(pres->get_view().selected_topology_scale, 10);
}

TEST_F(A_layout_presenter, Routes_update_layout_requests)
{
    pres->update_layout(layout_factory::gursoy_atun_desc);

    ASSERT_TRUE(pres->get_layout_updater().called);
    ASSERT_EQ(pres->get_layout_updater().type,
              layout_factory::gursoy_atun_desc);
}

TEST_F(A_layout_presenter, Routes_update_topology_requests)
{
    pres->update_topology(topology_factory::cube_desc, 1000000);

    ASSERT_TRUE(pres->get_space_updater().called);
    ASSERT_EQ(pres->get_space_updater().type, topology_factory::cube_desc);
    ASSERT_EQ(pres->get_space_updater().scale, 1000000);
}

} // namespace