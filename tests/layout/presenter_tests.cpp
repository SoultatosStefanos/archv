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
    void draw_vertex(vertex_id, coord, coord, coord)
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

    void on_layout_request(layout_request_listener) {}
    void on_topology_request(topology_request_listener) {}
};

struct dummy_update_layout
{
    using layout_response_listener = std::function<void(const lay::layout&)>;

    void on_layout_response(layout_response_listener) {}

    bool called = false;
    layout_factory::type_name given_type;

    void operator()(layout_factory::type_name t,
                    const architecture::graph&,
                    layout_factory::pointer&,
                    const topology_factory::pointer&)
    {
        called = true;
        given_type = t;
    }
};

struct dummy_update_topology
{
    using layout_response_listener =
        std::function<void(const lay::layout&, const topology&)>;

    void on_layout_response(layout_response_listener) {}

    bool called = false;
    topology_factory::type_name given_type;
    topology_factory::scale_type given_scale;

    void operator()(topology_factory::type_name t,
                    topology_factory::scale_type s,
                    const architecture::graph&,
                    layout_factory::pointer&,
                    topology_factory::pointer&)
    {
        called = true;
        given_type = t;
        given_scale = s;
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
    auto selection = "majestic layout";

    pres->update_layout(selection);

    ASSERT_TRUE(pres->get_layout_updater().called);
    ASSERT_EQ(pres->get_layout_updater().given_type, selection);
}

TEST_F(A_layout_presenter, Routes_update_topology_requests)
{
    auto selection = "giga hypercube";
    auto scale = 1000000;

    pres->update_topology(selection, scale);

    ASSERT_TRUE(pres->get_space_updater().called);
    ASSERT_EQ(pres->get_space_updater().given_type, selection);
    ASSERT_EQ(pres->get_space_updater().given_scale, scale);
}

} // namespace