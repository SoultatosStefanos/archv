// Contains the view presenter of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "factories.hpp"

#include <cassert>
#include <concepts>
#include <functional>
#include <unordered_map>

namespace layout
{

// ------------------------- Concepts -------------------------------- //

template <typename Class>
concept view_concept = requires(Class val, double num, std::string str)
{
    {val.draw_vertex(str, num, num, num)};
    /* {val.draw_edge(id, id, pos, pos, pos, pos, pos, pos)}; */ // TODO
    {val.update_layout_selection(str)};
    {val.update_topology_selection(str, num)};
    {val.on_layout_request(std::function<void(const std::string&)>())};
    {val.on_topology_request(
        std::function<void(const std::string&, double)>())};
};

template <typename Class>
concept update_layout_concept =
    std::invocable<Class, layout_factory::descriptor> && requires(Class val)
{
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

template <typename Class>
concept update_topology_concept =
    std::invocable<Class,
                   topology_factory::descriptor,
                   topology_factory::scale_type> && requires(Class val)
{
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

// ------------------------------------------------------------------- //

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
    using layout_type = layout_factory::descriptor;
    using topology_type = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using view = View;
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

    auto get_view() const -> const view& { return m_view; }
    auto get_view() -> view& { return m_view; }

    auto get_layout_updater() const -> const update_layout_service&
    {
        return m_update_layout;
    }
    auto get_layout_updater() -> update_layout_service&
    {
        return m_update_layout;
    }

    auto get_space_updater() const -> const update_topology_service&
    {
        return m_update_topology;
    }

    auto get_space_updater() -> update_topology_service&
    {
        return m_update_topology;
    }

    void update_view(const layout& l, const topology& s)
    {
        update_view_selections(l, s);
        update_view_vertices(l);
    }

    void update_layout(std::string selected_layout)
    {
        m_update_layout(std::move(selected_layout));
    }

    void update_topology(std::string selected_topology, double selected_scale)
    {
        m_update_topology(std::move(selected_topology), selected_scale);
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
