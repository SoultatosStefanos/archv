// Contains the view presenter of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "factories.hpp"

#include <cassert>
#include <concepts>
#include <functional>

namespace layout
{

// ------------------------- Concepts -------------------------------- //

template <typename Class>
concept view_concept =
    requires(Class val,
             typename Class::vertex_id id,
             typename Class::coord pos,
             typename Class::layout_selection layout_type,
             typename Class::topology_selection topology_type,
             typename Class::topology_scale_selection topology_scale,
             typename Class::layout_request_listener layout_listener,
             typename Class::topology_request_listener topology_listener)
{

    {val.draw_vertex(id, pos, pos, pos)};
    /* {val.draw_edge(id, id, pos, pos, pos, pos, pos, pos)}; */ // TODO
    {val.update_layout_selection(layout_type)};
    {val.update_topology_selection(topology_type, topology_scale)};
    {val.on_layout_request(layout_listener)};
    {val.on_topology_request(topology_listener)};
};

template <typename Class>
concept update_layout_concept =
    std::invocable<Class, layout_factory::type_name> &&
    requires(Class val, typename Class::layout_listener listener)
{
    {val.on_layout_response(listener)};
};

template <typename Class>
concept update_topology_concept =
    std::invocable<Class,
                   topology_factory::type_name,
                   topology_factory::scale_type> &&
    requires(Class val, typename Class::layout_listener listener)
{
    {val.on_layout_response(listener)};
};

// ------------------------------------------------------------------- //

// TODO Pass data to initialize the view selections

template <view_concept View,
          update_layout_concept UpdateLayoutService,
          update_topology_concept UpdateTopologyService>
requires std::move_constructible<View> &&
    std::move_constructible<UpdateLayoutService> &&
    std::move_constructible<UpdateTopologyService>
class presenter
{
public:
    using graph = architecture::graph;
    using view = View;
    using layout_selection = typename view::layout_selection;
    using topology_selection = typename view::topology_selection;
    using topology_scale_selection = typename view::topology_scale_selection;
    using update_layout_service = UpdateLayoutService;
    using update_topology_service = UpdateTopologyService;

    presenter(const graph& g,
              view v = view(),
              update_layout_service usecase1 = update_layout_service(),
              update_topology_service usecase2 = update_topology_service())
        : m_g{g},
          m_view{std::move(v)},
          m_update_layout{std::move(usecase1)},
          m_update_topology{std::move(usecase2)}
    {
        m_view.on_layout_request([this](auto type) { update_layout(type); });

        m_view.on_topology_request(
            [this](auto type, auto scale) { update_topology(type, scale); });

        m_update_layout.on_layout_response(
            [this](const auto& l, const auto& t) { update_view(l, t); });

        m_update_topology.on_layout_response(
            [this](const auto& l, const auto& t) { update_view(l, t); });
    }

    auto get_view() const -> const auto& { return m_view; }
    auto get_view() -> auto& { return m_view; }

    auto get_layout_updater() const -> const auto& { return m_update_layout; }
    auto get_layout_updater() -> auto& { return m_update_layout; }

    auto get_space_updater() const -> const auto& { return m_update_topology; }
    auto get_space_updater() -> auto& { return m_update_topology; }

    void update_view(const layout& l, const topology& s)
    {
        update_view_selections(l, s);
        update_view_vertices(l);
    }

    void update_layout(layout_selection type)
    {
        std::invoke(m_update_layout, type);
    }

    void update_topology(topology_selection type, topology_scale_selection s)
    {
        std::invoke(m_update_topology, type, s);
    }

private:
    void update_view_selections(const layout& l, const topology& s)
    {
        const auto selected_layout = typeid(l).name();
        const auto selected_space = typeid(s).name();

        m_view.update_layout_selection(selected_layout);
        m_view.update_topology_selection(selected_space, s.scale());
    }

    void update_view_vertices(const layout& l)
    {
        for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        {
            const auto& vertex_id = m_g[v].sym.id;
            m_view.draw_vertex(vertex_id, l.x(v), l.y(v), l.z(v));
        }
    }

    const graph& m_g;

    view m_view;

    update_layout_service m_update_layout;
    update_topology_service m_update_topology;
};

} // namespace layout

#endif // LAYOUT_PRESENTER_HPP
