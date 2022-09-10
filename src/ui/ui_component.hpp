// Contains an abstract ui component class for (user) configurable frontends.
// Soultatos Stefanos 2022

#ifndef UI_UI_COMPONENT_HPP
#define UI_UI_COMPONENT_HPP

#include <concepts>
#include <utility>

namespace ui
{

/***********************************************************
 * Concepts                                                *
 ***********************************************************/

template < typename Class, typename ConfigData >
concept config_interface
    = std::constructible_from< Class, ConfigData > && requires(Class val)
{
    // clang-format off
    { val.config_data() } -> std::same_as< const ConfigData& >;
    // clang-format on
};

/***********************************************************
 * UI Component                                            *
 ***********************************************************/

template < typename ConfigData, typename ConfigAPI >
requires config_interface< ConfigAPI, ConfigData >
class ui_component
{
    static_assert(std::is_copy_assignable_v< ConfigData >);

public:
    using config_data_type = ConfigData;
    using config_api_type = ConfigAPI;

    explicit ui_component(config_data_type cfg = config_data_type())
    : m_cfg { cfg }, m_default { cfg }, m_cfg_api { std::move(cfg) }
    {
    }

    ui_component(const ui_component&) = default;
    ui_component(ui_component&&) = default;

    virtual ~ui_component() = default;

    auto operator=(const ui_component&) -> ui_component& = default;
    auto operator=(ui_component&&) -> ui_component& = default;

    auto config_api() const -> const config_api_type& { return m_cfg_api; }
    auto config_api() -> config_api_type& { return m_cfg_api; }

    virtual auto draw(const config_data_type& cfg) const -> void = 0;

    auto begin_preview() const -> void { draw(config_api().config_data()); }
    auto end_preview() const -> void { draw(config_data()); }

    auto apply_configs() -> void { apply(config_api().config_data()); }
    auto restore_defaults() -> void { apply(default_data()); }

protected:
    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto default_data() const -> const config_data_type& { return m_default; }

private:
    auto apply(const config_data_type& cfg) { draw(config_data() = cfg); }

    config_data_type m_cfg, m_default;
    config_api_type m_cfg_api;
};

/***********************************************************
 * Type Traits                                             *
 ***********************************************************/

template < typename UIComponent >
struct ui_component_traits
{
    using config_data_type = typename UIComponent::config_data_type;
    using config_api_type = typename UIComponent::config_api_type;
};

} // namespace ui

#endif // UI_UI_COMPONENT_HPP
