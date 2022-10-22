#include "architecture/graph_interface.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace architecture;

namespace
{

using weights_backend_t = graph_interface::weights_backend_type;
using weights_config_t = weights_backend_t::config_data_type;
using layout_backend_t = graph_interface::layout_backend_type;
using layout_config_t = layout_backend_t::config_data_type;
using scaling_backend_t = graph_interface::scaling_backend_type;
using scaling_config_t = scaling_backend_t::config_data_type;

constexpr auto id0 = "id0";
constexpr auto id1 = "id1";

constexpr auto dependency0 = "dependency0";

constexpr auto weight0 = 10;

constexpr auto layout_type = layout::gursoy_atun_id;
constexpr auto topology_type = layout::cube_id;
constexpr auto scale = 100;

inline auto make_structure(std::string id)
{
    structure s;
    s.sym.id = std::move(id);
    return s;
}

inline auto make_symbol_table()
{
    symbol_table st;
    st.insert(make_structure(id0));
    st.insert(make_structure(id1));
    return st;
}

inline auto make_graph()
{
    graph g;
    const auto v0 = boost::add_vertex(id0, g);
    const auto v1 = boost::add_vertex(id1, g);
    boost::add_edge(v0, v1, dependency0, g);
    return g;
}

inline auto make_weights_cfg()
{
    return weights_config_t { { dependency0, weight0 } };
}

inline auto make_layout_cfg()
{
    return layout_config_t { .layouts = { std::string(layout_type) },
                             .topologies = { std::string(topology_type) },
                             .layout = std::string(layout_type),
                             .topology = std::string(topology_type),
                             .scale = scale };
}

inline auto make_scaling_cfg()
{
    return scaling_config_t { { metadata_counter::fields_tag,
                                scaling::make_xyz_factor(1, true) } };
}

class a_graph_interface : public Test
{
protected:
    void SetUp() override
    {
        iface = std::make_unique< graph_interface >(
            make_symbol_table(),
            make_graph(),
            make_weights_cfg(),
            make_layout_cfg(),
            make_scaling_cfg());
    }

    std::unique_ptr< graph_interface > iface;
};

TEST_F(a_graph_interface, can_be_queried_for_vertex_ids)
{
    assert(boost::num_vertices(iface->get_graph()) == 2);
    const auto [first, last] = boost::vertices(iface->get_graph());
    const auto v0 = *first;
    const auto v1 = *(first + 1);

    ASSERT_EQ(boost::get(iface->vertex_id(), v0), id0);
    ASSERT_EQ(boost::get(iface->vertex_id(), v1), id1);
}

TEST_F(a_graph_interface, can_be_queried_for_vertex_pos)
{
    // NOTE: non deterministic layout
    assert(boost::num_vertices(iface->get_graph()) == 2);
    const auto [first, last] = boost::vertices(iface->get_graph());
    const auto v0 = *first;
    const auto v1 = *(first + 1);
    const auto& curr_layout = iface->layout_backend().get_layout();
    const auto [x0, y0, z0] = boost::get(iface->vertex_position(), v0);
    const auto [x1, y1, z1] = boost::get(iface->vertex_position(), v1);

    ASSERT_EQ(x0, curr_layout.x(v0));
    ASSERT_EQ(y0, curr_layout.y(v0));
    ASSERT_EQ(z0, curr_layout.z(v0));
    ASSERT_EQ(x1, curr_layout.x(v1));
    ASSERT_EQ(y1, curr_layout.y(v1));
    ASSERT_EQ(z1, curr_layout.z(v1));
}

TEST_F(a_graph_interface, can_be_queried_for_vertex_scale)
{
    assert(boost::num_vertices(iface->get_graph()) == 2);
    const auto [first, last] = boost::vertices(iface->get_graph());
    const auto v0 = *first;
    const auto v1 = *(first + 1);

    ASSERT_EQ(
        boost::get(iface->vertex_scale(), v0),
        scaling::make_scale(0, 0, 0)); // 0 fields
    ASSERT_EQ(
        boost::get(iface->vertex_scale(), v1),
        scaling::make_scale(0, 0, 0)); // 0 fields
}

TEST_F(a_graph_interface, can_be_queried_for_edge_dependencies)
{
    assert(boost::num_edges(iface->get_graph()) == 1);
    const auto [first, last] = boost::edges(iface->get_graph());
    const auto e0 = *first;

    ASSERT_EQ(boost::get(iface->edge_dependency(), e0), dependency0);
}

TEST_F(a_graph_interface, can_be_queried_for_edge_weights)
{
    assert(boost::num_edges(iface->get_graph()) == 1);
    const auto [first, last] = boost::edges(iface->get_graph());
    const auto e0 = *first;

    ASSERT_EQ(boost::get(iface->edge_weight(), e0), weight0);
}

} // namespace