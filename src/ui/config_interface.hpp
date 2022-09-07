// Contains a class responsible for the user interactions with configurable ui
// properties.
// Soultatos Stefanos 2022

#ifndef UI_CONFIG_INTERFACE_HPP
#define UI_CONFIG_INTERFACE_HPP

#include <type_traits>
#include <utility>

namespace ui
{

template < typename ConfigData >
class config_interface
{
    static_assert(std::is_aggregate_v< ConfigData >);

public:
    using config_data_type = ConfigData;

    explicit config_interface(config_data_type config = config_data_type())
    : m_config { std::move(config) }
    {
    }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

private:
    config_data_type m_config;
};

} // namespace ui

#endif // UI_CONFIG_INTERFACE_HPP
