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

    void draw_vertex(vertex_id, coord, coord, coord) {}

    void update_layout_selection(layout_selection) {}
    void update_topology_selection(topology_selection, topology_scale_selection)
    {}

    void on_layout_request(layout_request_listener) {}
    void on_topology_request(topology_request_listener) {}
};

struct dummy_update_layout
{
    using layout_response_listener = std::function<void(const lay::layout&)>;

    void on_layout_response(layout_response_listener) {}

    void operator()(layout_factory::type_name,
                    const architecture::graph&,
                    layout_factory::pointer&,
                    const topology_factory::pointer&)
    {}
};

struct dummy_update_topology
{
    using layout_response_listener =
        std::function<void(const lay::layout&, const topology&)>;

    void on_layout_response(layout_response_listener) {}

    void operator()(topology_factory::type_name,
                    topology_factory::scale_type,
                    const architecture::graph&,
                    layout_factory::pointer&,
                    topology_factory::pointer&)
    {}
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

// TEST_F(A_layout_presenter,
//        Calls_the_view_with_update_layout_view_vertices_num_times)
// {
//     auto s = cube(3);
//     auto l = std::make_unique<gursoy_atun_layout>(g, s);
//     layout_response_event event{.curr = *l};

//     EXPECT_CALL(pres->ui)

//     EXPECT_CALL(mock,
//                 draw_vertex(testing::_, testing::_, testing::_, testing::_))
//         .Times(vertices_num);

//     pipeline->post(event);
// }

} // namespace