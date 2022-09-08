// Contains a module for managing layout types via ids at run/compile time.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_PLUGIN_HPP
#define LAYOUT_LAYOUT_PLUGIN_HPP

#include "layout.hpp"
#include "layout_visitor.hpp"

#include <array>
#include <string>

namespace layout
{

namespace layout_plugin
{
    using identifier = std::string;

    static constexpr auto gursoy_atun_id = "Gursoy Atun";

    static constexpr auto enumeration = std::array { gursoy_atun_id };

    constexpr auto enumerates(const identifier& tag) -> bool
    {
        constexpr auto& set = enumeration;
        return std::find(std::begin(set), std::end(set), tag) != std::end(set);
    }

    namespace detail
    {
        template < typename Graph >
        class layout_identifier : public layout_visitor< Graph >
        {
        public:
            using base = layout_visitor< Graph >;
            using gursoy_atun_type = typename base::gursoy_atun_type;

            explicit layout_identifier(identifier& i) : m_i { i } { }
            ~layout_identifier() override = default;

            auto visit(const gursoy_atun_type&) const -> void
            {
                m_i = gursoy_atun_id;
            }

        private:
            identifier& m_i;
        };

    } // namespace detail

    template < typename Graph >
    auto identify(const layout< Graph >& l) -> identifier
    {
        identifier res;
        l.accept(detail::layout_identifier< Graph >(res));
        assert(enumerates(res));
        return res;
    }

} // namespace layout_plugin

} // namespace layout

#endif // LAYOUT_LAYOUT_PLUGIN_HPP
