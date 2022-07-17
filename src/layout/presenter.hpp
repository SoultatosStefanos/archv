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
    std::invocable<Class,
                   layout_factory::type_name,
                   const architecture::graph&,
                   layout_factory::pointer&,
                   const topology_factory::pointer&> &&
    requires(Class val, typename Class::layout_response_listener listener)
{
    {val.on_layout_response(listener)};
};

template <typename Class>
concept update_topology_concept = std::invocable<Class,
                                                 topology_factory::type_name,
                                                 topology_factory::scale_type,
                                                 const architecture::graph&,
                                                 layout_factory::pointer&,
                                                 topology_factory::pointer&> &&
    requires(Class val, typename Class::layout_response_listener listener)
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
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using view = View;
    using layout_selection = typename view::layout_selection;
    using topology_selection = typename view::topology_selection;
    using topology_scale_selection = typename view::topology_scale_selection;
    using update_layout_service = UpdateLayoutService;
    using update_topology_service = UpdateTopologyService;

    presenter(const graph& g,
              layout_pointer layout = nullptr,
              topology_pointer topology = nullptr,
              view v = view(),
              update_layout_service usecase1 = update_layout_service(),
              update_topology_service usecase2 = update_topology_service())
        : m_g{g},
          m_layout{std::move(layout)},
          m_space{std::move(topology)},
          m_view{std::move(v)},
          m_update_layout{std::move(usecase1)},
          m_update_topology{std::move(usecase2)}
    {
        m_view.on_layout_request([this](auto type) { update_layout(type); });
        m_view.on_topology_request(
            [this](auto type, auto scale) { update_topology(type, scale); });

        m_update_layout.on_layout_response(
            [this](const auto& l) { update_view(l); });

        m_update_topology.on_layout_response(
            [this](const auto& l, const auto& t) { update_view(l, t); });
    }

    auto ui() const -> const view& { return m_view; }
    auto ui() -> view& { return m_view; }

    void update_view()
    {
        assert(m_layout);
        update_view(*m_layout);
    }

    void update_view(const layout& l)
    {
        assert(m_space);
        update_view_selections(l, *m_space);
        update_view_vertices(l);
    }

    void update_view(const layout& l, const topology& s)
    {
        update_view_selections(l, s);
        update_view_vertices(l);
    }

    void update_layout(layout_selection type)
    {
        m_update_layout(type, m_g, m_layout, m_space);
    }

    void update_topology(topology_selection type, topology_scale_selection s)
    {
        m_update_topology(type, s, m_g, m_layout, m_space);
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
    layout_pointer m_layout;
    topology_pointer m_space;

    view m_view;

    update_layout_service m_update_layout;
    update_topology_service m_update_topology;
};

} // namespace layout

#endif // LAYOUT_PRESENTER_HPP
