// Contains some ui component specific type traits.
// Soultatos Stefanos 2022

#ifndef UI_UI_TRAITS_HPP
#define UI_UI_TRAITS_HPP

#include "ui_concepts.hpp"

namespace ui
{

template < typename UIComponent >
requires interactive_configurable< UIComponent >
struct ui_component_traits
{
    using config_data_type = typename UIComponent::config_data_type;
    using config_api_type = typename UIComponent::config_api_type;
};

} // namespace ui

#endif // UI_UI_TRAITS_HPP
