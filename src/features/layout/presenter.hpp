// Contains the view presenter of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "dependencies/graph.hpp"
#include "factories.hpp"

#include <cassert>
#include <concepts>
#include <functional>
#include <unordered_map>

namespace features::layout
{

/***********************************************************
 * Presenter Concepts                                      *
 ***********************************************************/

template <typename Class>
concept view_concept = requires(Class val)
{
    {val.draw_vertex(std::string(), double(), double(), double())};
    /* {val.draw_edge(id, id, pos, pos, pos, pos, pos, pos)}; */ // TODO
    {val.update_layout_selection(std::string())};
    {val.update_topology_selection(std::string(), double())};
    {val.on_layout_request(std::function<void(const std::string&)>())};
    {val.on_topology_request(
        std::function<void(const std::string&, double)>())};
};

template <typename Class>
concept update_layout_concept = requires(Class val)
{
    {val.execute(std::string())};
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

template <typename Class>
concept update_topology_concept = requires(Class val)
{
    {val.execute(std::string(), topology_factory::scale_type())};
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

/***********************************************************
 * Presenter                                               *
 ***********************************************************/

template <view_concept View,
          update_layout_concept UpdateLayoutService,
          update_topology_concept UpdateTopologyService>
class presenter
{
public:
    using graph = dependencies::graph;
    using vertex_id_map = dependencies::vertex_id_map;
    using layout_type = layout_factory::descriptor;
    using topology_type = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using view = View;
    using update_layout_service = UpdateLayoutService;
    using update_topology_service = UpdateTopologyService;

    presenter(const graph& g,
              vertex_id_map vertex_id,
              view& v,
              update_layout_service& usecase1,
              update_topology_service& usecase2)
        : m_g{g},
          m_vertex_id{vertex_id},
          m_view{v},
          m_update_layout{usecase1},
          m_update_topology{usecase2}
    {
        m_view.on_layout_request(
            [this](const auto& type) { update_layout(type); });

        m_view.on_topology_request([this](const auto& type, auto scale) {
            update_topology(type, scale);
        });

        m_update_layout.on_layout_response(
            [this](const auto& l, const auto& t) { update_view(l, t); });

        m_update_topology.on_layout_response(
            [this](const auto& l, const auto& t) { update_view(l, t); });
    }

    void update_view(const layout& l, const topology& s)
    {
        update_view_selections(l, s);
        update_view_vertices(l);
    }

    void update_layout(std::string selected_layout)
    {
        m_update_layout.execute(std::move(selected_layout));
    }

    void update_topology(std::string selected_topology, double selected_scale)
    {
        m_update_topology.execute(std::move(selected_topology), selected_scale);
    }

private:
    void update_view_selections(const layout& l, const topology& s)
    {
        m_view.update_layout_selection(l.desc());
        m_view.update_topology_selection(s.desc(), s.scale());
    }

    void update_view_vertices(const layout& l)
    {
        for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        {
            const auto& vertex_id = boost::get(m_vertex_id, v);
            m_view.draw_vertex(vertex_id, l.x(v), l.y(v), l.z(v));
        }
    }

    const graph& m_g;
    vertex_id_map m_vertex_id;

    view& m_view;

    update_layout_service& m_update_layout;
    update_topology_service& m_update_topology;
};

} // namespace features::layout

#endif // LAYOUT_PRESENTER_HPP
