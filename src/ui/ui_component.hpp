// Contains an ui_component configurable helper template.
// Soultatos Stefanos 2022

#ifndef UI_ui_component_HPP
#define UI_ui_component_HPP

#include "config_interface.hpp"

#include <type_traits>
#include <utility>

namespace ui
{

// UI, configurable helper template.
// For aggregate properties.
template < typename ConfigData >
class ui_component
{
    static_assert(std::is_aggregate_v< ConfigData >);

public:
    using config_data_type = ConfigData;
    using config_api_type = config_interface< ConfigData >;

    explicit ui_component(config_data_type config = config_data_type())
    : m_config { config }, m_config_api { std::move(config) }
    {
    }

    ui_component(const ui_component&) = default;
    ui_component(ui_component&&) = default;
    virtual ~ui_component() = default;

    auto operator=(const ui_component&) -> ui_component& = default;
    auto operator=(ui_component&&) -> ui_component& = default;

    auto config_api() const -> const auto& { return m_config_api; }
    auto config_api() -> auto& { return m_config_api; }

    auto apply_configs() -> void
    {
        setup(config_data() = config_api().config_data());
    }

    auto begin_preview() -> void { setup(config_api().config_data()); }
    auto end_preview() -> void { setup(config_data()); }

protected:
    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

    virtual auto setup(const config_data_type& config) -> void = 0;

private:
    config_data_type m_config;
    config_api_type m_config_api;
};

} // namespace ui

#endif // UI_ui_component_HPP
