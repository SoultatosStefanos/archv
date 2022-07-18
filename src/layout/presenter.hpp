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
concept view_concept =
    requires(Class val,
             typename Class::vertex_id id,
             typename Class::coord pos,
             typename Class::layout_selection layout_type,
             typename Class::topology_selection topology_type,
             typename Class::topology_scale_selection topology_scale,
             typename Class::layout_request_listener layout_listener,
             typename Class::topology_request_listener topology_listener) {
        {
            val.draw_vertex(id, pos, pos, pos)
        };
        /* {val.draw_edge(id, id, pos, pos, pos, pos, pos, pos)}; */ // TODO
        {
            val.update_layout_selection(layout_type)
        };
        {
            val.update_topology_selection(topology_type, topology_scale)
        };
        {
            val.on_layout_request(layout_listener)
        };
        {
            val.on_topology_request(topology_listener)
        };
    };

template <typename Class>
concept update_layout_concept =
    std::invocable<Class, layout_factory::type_name> &&
    requires(Class val, typename Class::layout_listener listener) {
        {
            val.on_layout_response(listener)
        };
    };

template <typename Class>
concept update_topology_concept =
    std::invocable<Class,
                   topology_factory::type_name,
                   topology_factory::scale_type> &&
    requires(Class val, typename Class::layout_listener listener) {
        {
            val.on_layout_response(listener)
        };
    };

template <typename Class, typename View>
concept layout_formatter_concept =
    requires(Class val,
             layout_factory::type_name type,
             typename View::layout_selection selection) {
        // clang-format off
        { val.format(type) } -> std::same_as<decltype(selection)>;
        { val.unformat(selection) } -> std::same_as<decltype(type)>;
        // clang-format on
    };

template <typename Class, typename View>
concept topology_formatter_concept =
    requires(Class val,
             topology_factory::type_name type,
             typename View::topology_selection selection) {
        // clang-format off
        { val.format(type) } -> std::same_as<decltype(selection)>;
        { val.unformat(selection) } -> std::same_as<decltype(type)>;
        // clang-format on
    };

// ------------------------------------------------------------------- //

// ------------------------ Formatting ------------------------------- //

namespace detail
{
    template <view_concept View>
    struct layout_formatter
    {
        using layout_type = layout_factory::type_name;
        using layout_selection = typename View::layout_selection;

        static auto format(layout_type t) -> layout_selection
        {
            using table = std::unordered_map<layout_type, layout_selection>;

            static const table cache{{layout_type::gursoy_atun, "Gursoy Atun"}};
            assert(cache.contains(t));
            return cache.at(t);
        }

        static auto unformat(const layout_selection& s) -> layout_type
        {
            using table = std::unordered_map<layout_selection, layout_type>;

            static const table cache{{"Gursoy Atun", layout_type::gursoy_atun}};
            assert(cache.contains(s));
            return cache.at(s);
        }
    };

    template <view_concept View>
    struct topology_formatter
    {
        using topology_type = topology_factory::type_name;
        using topology_selection = typename View::topology_selection;

        static auto format(topology_type t) -> topology_selection
        {
            using table = std::unordered_map<topology_type, topology_selection>;

            static const table cache{
                {topology_factory::type_name::cube, "Cube"},
                {topology_factory::type_name::sphere, "Sphere"}};

            assert(cache.contains(t));
            return cache.at(t);
        }

        static auto unformat(const topology_selection& s) -> topology_type
        {
            using table = std::unordered_map<topology_selection, topology_type>;

            static const table cache{
                {"Cube", topology_factory::type_name::cube},
                {"Sphere", topology_factory::type_name::sphere}};

            assert(cache.contains(s));
            return cache.at(s);
        }
    };

} // namespace detail

// ------------------------------------------------------------------- //

template <view_concept View,
          update_layout_concept UpdateLayoutService,
          update_topology_concept UpdateTopologyService,
          typename LayoutFormatter = detail::layout_formatter<View>,
          typename TopologyFormatter = detail::topology_formatter<View>>
requires std::move_constructible<View> &&
         std::move_constructible<UpdateLayoutService> &&
         std::move_constructible<UpdateTopologyService> &&
         layout_formatter_concept<LayoutFormatter, View> &&
         topology_formatter_concept<TopologyFormatter, View> &&
         std::move_constructible<LayoutFormatter> &&
         std::move_constructible<TopologyFormatter>
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
    using layout_type = layout_factory::type_name;
    using topology_type = topology_factory::type_name;
    using topology_scale = topology_factory::scale_type;
    using layout_formatter = LayoutFormatter;
    using topology_formatter = TopologyFormatter;

    presenter(const graph& g,
              view v = view(),
              update_layout_service usecase1 = update_layout_service(),
              update_topology_service usecase2 = update_topology_service(),
              layout_formatter lformatter = layout_formatter(),
              topology_formatter tformatter = topology_formatter())
        : m_g{g},
          m_view{std::move(v)},
          m_update_layout{std::move(usecase1)},
          m_update_topology{std::move(usecase2)},
          m_layout_formatter{std::move(lformatter)},
          m_topology_formatter{std::move(tformatter)}
    {
        m_view.on_layout_request([this](auto type) { update_layout(type); });

        m_view.on_topology_request(
            [this](auto type, auto scale) { update_topology(type, scale); });

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

    auto get_layout_formatter() const -> const layout_formatter&
    {
        return m_layout_formatter;
    }

    auto get_layout_formatter() -> layout_formatter&
    {
        return m_layout_formatter;
    }

    auto get_topology_formatter() const -> const topology_formatter&
    {
        return m_topology_formatter;
    }

    auto get_topology_formatter() -> topology_formatter&
    {
        return m_topology_formatter;
    }

    void update_view(const layout& l, const topology& s)
    {
        update_view_selections(l, s);
        update_view_vertices(l);
    }

    void update_layout(const layout_selection& selected_layout)
    {
        const auto type = m_layout_formatter.unformat(selected_layout);
        m_update_layout(type);
    }

    void update_topology(const topology_selection& selected_topology,
                         topology_scale_selection selected_scale)
    {
        const auto type = m_topology_formatter.unformat(selected_topology);
        m_update_topology(type, selected_scale);
    }

private:
    void update_view_selections(const layout& l, const topology& s)
    {
        const auto ltype = layout_factory::resolve_type(l);
        const auto stype = topology_factory::resolve_type(s);
        auto&& selected_layout = m_layout_formatter.format(ltype);
        auto&& selected_space = m_topology_formatter.format(stype);

        m_view.update_layout_selection(std::move(selected_layout));
        m_view.update_topology_selection(std::move(selected_space), s.scale());
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

    layout_formatter m_layout_formatter;
    topology_formatter m_topology_formatter;
};

} // namespace layout

#endif // LAYOUT_PRESENTER_HPP
