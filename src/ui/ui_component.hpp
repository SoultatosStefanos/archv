// Contains some utility functions for abstract ui components.
// Soultatos Stefanos 2022

#ifndef UI_UI_COMPONENT_HPP
#define UI_UI_COMPONENT_HPP

#include "ui_concepts.hpp"

namespace ui
{

template < typename UIComponent >
requires interactive_configurable< UIComponent > && drawable< UIComponent >
inline auto begin_preview(UIComponent& c) -> void
{
    c.draw(c.config_api().config_data());
}

template < typename UIComponent >
requires configurable< UIComponent > && drawable< UIComponent >
inline auto end_preview(UIComponent& c) -> void
{
    c.draw(c.config_data());
}

template < typename UIComponent >
requires interactive_configurable< UIComponent > && drawable< UIComponent >
inline auto apply_configs(UIComponent& c) -> void
{
    c.draw(c.config_data() = c.config_api().config_data());
}

template < typename UIComponent >
requires default_configurable< UIComponent > && drawable< UIComponent >
inline auto restore_defaults(UIComponent& c) -> void
{
    c.draw(c.config_data() = c.default_data());
}

} // namespace ui

#endif // UI_UI_COMPONENT_HPP
