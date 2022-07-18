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
    std::invocable<Class, layout_factory::type_name> && requires(Class val)
{
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

template <typename Class>
concept update_topology_concept =
    std::invocable<Class,
                   topology_factory::type_name,
                   topology_factory::scale_type> && requires(Class val)
{
    {val.on_layout_response(
        std::function<void(const layout&, const topology&)>())};
};

template <typename Class>
concept layout_formatter_concept = requires(Class val,
                                            layout_factory::type_name type,
                                            std::string selection)
{
    // clang-format off
        { val.format(type) } -> std::same_as<decltype(selection)>;
        { val.unformat(selection) } -> std::same_as<decltype(type)>;
    // clang-format on
};

template <typename Class>
concept space_formatter_concept = requires(Class val,
                                           topology_factory::type_name type,
                                           std::string selection)
{
    // clang-format off
        { val.format(type) } -> std::same_as<decltype(selection)>;
        { val.unformat(selection) } -> std::same_as<decltype(type)>;
    // clang-format on
};

// ------------------------------------------------------------------- //

// ------------------------ Formatting ------------------------------- //

namespace detail
{
    struct layout_formatter
    {
        using layout_type = layout_factory::type_name;

        static auto format(layout_type t) -> std::string
        {
            using table = std::unordered_map<layout_type, std::string>;

            static const table cache{{layout_type::gursoy_atun, "Gursoy Atun"}};
            assert(cache.contains(t));
            return cache.at(t);
        }

        static auto unformat(const std::string& s) -> layout_type
        {
            using table = std::unordered_map<std::string, layout_type>;

            static const table cache{{"Gursoy Atun", layout_type::gursoy_atun}};
            assert(cache.contains(s));
            return cache.at(s);
        }
    };

    struct space_formatter
    {
        using topology_type = topology_factory::type_name;

        static auto format(topology_type t) -> std::string
        {
            using table = std::unordered_map<topology_type, std::string>;

            static const table cache{
                {topology_factory::type_name::cube, "Cube"},
                {topology_factory::type_name::sphere, "Sphere"}};

            assert(cache.contains(t));
            return cache.at(t);
        }

        static auto unformat(const std::string& s) -> topology_type
        {
            using table = std::unordered_map<std::string, topology_type>;

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
          layout_formatter_concept LayoutFormatter = detail::layout_formatter,
          space_formatter_concept TopologyFormatter = detail::space_formatter>
requires std::move_constructible<View> &&
    std::move_constructible<UpdateLayoutService> &&
    std::move_constructible<UpdateTopologyService> &&
    std::move_constructible<LayoutFormatter> &&
    std::move_constructible<TopologyFormatter>
class presenter
{
public:
    using graph = architecture::graph;
    using layout_type = layout_factory::type_name;
    using topology_type = topology_factory::type_name;
    using topology_scale = topology_factory::scale_type;
    using view = View;
    using update_layout_service = UpdateLayoutService;
    using update_topology_service = UpdateTopologyService;
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

    void update_layout(const std::string& selected_layout)
    {
        const auto type = m_layout_formatter.unformat(selected_layout);
        m_update_layout(type);
    }

    void update_topology(const std::string& selected_topology,
                         double selected_scale)
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
