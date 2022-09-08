// Contains some ui concepts.
// Soultatos Stefanos 2022.

#ifndef UI_CONCEPTS_HPP
#define UI_CONCEPTS_HPP

#include <concepts>

namespace ui
{

template < typename Class >
concept configurable = requires(Class val)
{
    { val.config_data() };
    { val.config_data() = decltype(val.config_data())() };
};

template < typename Class >
concept interactive_configurable = configurable< Class > && requires(Class val)
{
    { val.config_api() };
    // clang-format off
    { val.config_api().config_data() } 
		->
	std::same_as< decltype(val.config_data()) >;
    // clang-format on
};

template < typename Class >
concept drawable = configurable< Class > && requires(Class val)
{
    { val.draw(val.config_data()) };
};

template < typename Class >
concept ui_component = interactive_configurable< Class > && drawable< Class >;

} // namespace ui

#endif // UI_CONCEPTS_HPP
