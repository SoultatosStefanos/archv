// Contains a private module for the interactive.hpp header.
// Soultatos Stefanos 2022

#ifndef CONFIG_DETAIL_INTERACTIVE_HPP
#define CONFIG_DETAIL_INTERACTIVE_HPP

#include <utility>

namespace config::detail
{

template < typename ConfigData >
class config_api
{
public:
    using config_data_type = ConfigData;

    explicit config_api(config_data_type config = config_data_type())
    : m_config { std::move(config) }
    {
    }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

private:
    config_data_type m_config;
};

} // namespace config::detail

#endif // CONFIG_DETAIL_INTERACTIVE_HPP
