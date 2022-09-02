// Contains an interactive configurable helper template.
// Soultatos Stefanos 2022

#ifndef CONFIG_INTERACTIVE_HPP
#define CONFIG_INTERACTIVE_HPP

#include "detail/interactive.hpp"

#include <type_traits>
#include <utility>

namespace config
{

// Interactive, configurable helper template.
// For aggregate properties.
template < typename ConfigData >
class interactive
{
    static_assert(std::is_aggregate_v< ConfigData >);

public:
    using config_data_type = ConfigData;

    explicit interactive(config_data_type config = config_data_type())
    : m_config { config }, m_config_api { std::move(config) }
    {
    }

    interactive(const interactive&) = default;
    interactive(interactive&&) = default;
    virtual ~interactive() = default;

    auto operator=(const interactive&) -> interactive& = default;
    auto operator=(interactive&&) -> interactive& = default;

    auto configs() const -> const auto& { return m_config_api.config_data(); }
    auto configs() -> auto& { return m_config_api.config_data(); }

    auto apply_configs() -> void { setup_configs(config_data() = configs()); }

    auto begin_preview() -> void { setup_configs(configs()); }
    auto end_preview() -> void { setup_configs(config_data()); }

protected:
    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

    virtual auto setup_configs(const config_data_type& config) -> void = 0;

private:
    using config_api_type = detail::config_api< config_data_type >;

    config_data_type m_config;
    config_api_type m_config_api;
};

} // namespace config

#endif // CONFIG_INTERACTIVE_HPP
