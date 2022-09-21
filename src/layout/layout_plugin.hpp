// Contains a module for managing layout types via ids at run/compile time.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_PLUGIN_HPP
#define LAYOUT_LAYOUT_PLUGIN_HPP

#include "layout.hpp"
#include "layout_visitor.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <string_view>

namespace layout
{

using layout_id = std::string_view;

constexpr layout_id gursoy_atun_id = "Gursoy Atun";

constexpr auto layout_ids = std::array { gursoy_atun_id };

constexpr auto is_layout_plugged_in(layout_id id) -> bool
{
    constexpr auto& set = layout_ids;
    return std::find(std::begin(set), std::end(set), id) != std::end(set);
}

static_assert(is_layout_plugged_in(gursoy_atun_id));

namespace detail
{
    template < typename Graph >
    class layout_identifier : public layout_visitor< Graph >
    {
    public:
        using base = layout_visitor< Graph >;
        using gursoy_atun_type = typename base::gursoy_atun_type;

        explicit layout_identifier(layout_id& i) : m_i { i } { }
        ~layout_identifier() override = default;

        auto visit(const gursoy_atun_type&) const -> void
        {
            m_i = gursoy_atun_id;
        }

    private:
        layout_id& m_i;
    };

} // namespace detail

template < typename Layout >
requires std::derived_from< Layout, layout< typename Layout::graph_type > >
auto identify(const Layout& l) -> layout_id
{
    using graph_type = typename Layout::graph_type;

    layout_id res;
    l.accept(detail::layout_identifier< graph_type >(res));
    assert(is_layout_plugged_in(res));
    return res;
}

} // namespace layout

#endif // LAYOUT_LAYOUT_PLUGIN_HPP
