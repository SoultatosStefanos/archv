// Contains some utility functions for abstract ui components.
// Soultatos Stefanos 2022

#ifndef UI_UI_COMPONENT_HPP
#define UI_UI_COMPONENT_HPP

#include "ui_concepts.hpp" // for drawable, configurable, etc

#include <concepts> // for move_assignable
#include <utility>  // for move

namespace ui
{

// clang-format off

template < typename UIComponent >
requires configurable< UIComponent > && drawable< UIComponent >
inline auto start(UIComponent& c) -> void
{
    c.draw(c.config_data());
}

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
    c.config_api().config_data() = c.default_data();
}

template < typename UIComponent >
requires configurable< UIComponent > &&
         drawable< UIComponent > && 
         std::movable< typename UIComponent::config_data_type >
inline auto import_configs(
             UIComponent& c,
             typename UIComponent::config_data_type cfg) -> void
{
    c.draw(c.config_data() = std::move(cfg));
}

template < typename UIComponent >
requires interactive_configurable< UIComponent > &&
         drawable< UIComponent > && 
         std::movable< typename UIComponent::config_data_type >
inline auto import_configs(
             UIComponent& c,
             typename UIComponent::config_data_type cfg) -> void
{
    c.config_api().config_data() = std::move(cfg);
    apply_configs(c);
}

template < typename UIComponent >
requires configurable< UIComponent >
inline auto export_configs(const UIComponent& c) 
    -> typename UIComponent::config_data_type 
{
    // NOTE: UI components configs are always up to date.
    return c.config_data();
}

// clang-format on

} // namespace ui

#endif // UI_UI_COMPONENT_HPP
