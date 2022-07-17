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
    using vertex_id = std::string;
    using coord = long double;
    using layout_selection = const char*;
    using topology_selection = const char*;
    using topology_scale_selection = double;
    using layout_request_listener = std::function<void(layout_selection)>;
    using topology_request_listener =
        std::function<void(topology_selection, topology_scale_selection)>;

    int draw_vertex_called_times = 0;
    void draw_vertex(const vertex_id&, coord, coord, coord)
    {
        ++draw_vertex_called_times;
    }

    layout_selection selected_layout = nullptr;
    void update_layout_selection(layout_selection l) { selected_layout = l; }

    topology_selection selected_topology = nullptr;
    topology_scale_selection selected_topology_scale;
    void update_topology_selection(topology_selection space,
                                   topology_scale_selection scale)
    {
        selected_topology = space;
        selected_topology_scale = scale;
    }

    void on_layout_request(const layout_request_listener&) {}
    void on_topology_request(const topology_request_listener&) {}
};

struct dummy_update_layout
{
    using layout_listener =
        std::function<void(const lay::layout&, const topology&)>;

    void on_layout_response(const layout_listener&) {}

    bool called = false;
    void operator()(layout_factory::type_name) { called = true; }
};

struct dummy_update_topology
{
    using layout_listener =
        std::function<void(const lay::layout&, const topology&)>;

    void on_layout_response(const layout_listener&) {}

    bool called = false;
    void operator()(topology_factory::type_name, topology_factory::scale_type)
    {
        called = true;
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
       Given_a_layout_and_topology_sets_the_selected_elements_at_view)
{
    const auto space = cube(10);
    const auto lay = gursoy_atun_layout(g, space);

    pres->update_view(lay, space);

    ASSERT_NE(pres->get_view().selected_layout, nullptr);
    ASSERT_NE(pres->get_view().selected_topology, nullptr);
    ASSERT_EQ(pres->get_view().selected_topology_scale, 10);
}

TEST_F(A_layout_presenter, Routes_update_layout_requests)
{
    pres->update_layout("majestic layout");

    ASSERT_TRUE(pres->get_layout_updater().called);
}

TEST_F(A_layout_presenter, Routes_update_topology_requests)
{
    pres->update_topology("giga hypercube", 1000000);

    ASSERT_TRUE(pres->get_space_updater().called);
}

} // namespace