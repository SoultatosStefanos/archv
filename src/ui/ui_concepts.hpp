// Contains some ui component specific concepts.
// Soultatos Stefanos 2022

#ifndef UI_UI_CONCEPTS_HPP
#define UI_UI_CONCEPTS_HPP

namespace ui
{

// clang-format off

template < typename Class >
concept configurable = requires(Class val)
{
    { val.config_data() };
    { val.config_data() = val.config_data() };
};

template < typename Class >
concept default_configurable = configurable< Class > && requires(Class val)
{
    { val.default_data() };
    { val.config_data() = val.default_data() };
};

template < typename Class >
concept interactive_configurable = configurable< Class > && requires(Class val)
{
    { val.config_api() };
    { val.config_api().config_data() };
    { val.config_api().config_data() = val.config_data() };
    { val.config_data() = val.config_api().config_data() };
};

template < typename Class >
concept drawable = configurable< Class > && requires(Class val)
{
    { val.draw(val.config_data()) };
};

// clang-format on

} // namespace ui

#endif // UI_UI_CONCEPTS_HPP
