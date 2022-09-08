// Contains some utility functions for abstract ui components.
// Soultatos Stefanos 2022

#ifndef UI_UI_HPP
#define UI_UI_HPP

#include "concepts.hpp"

namespace ui
{

template < ui_component UIComponent >
inline auto begin_preview(UIComponent& c) -> void
{
    c.draw(c.config_api().config_data());
}

template < ui_component UIComponent >
inline auto end_preview(UIComponent& c) -> void
{
    c.draw(c.config_data());
}

template < ui_component UIComponent >
inline auto apply_configs(UIComponent& c) -> void
{
    c.draw(c.config_data() = c.config_api().config_data());
}

} // namespace ui

#endif // UI_UI_HPP
