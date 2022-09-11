// Contains classes responsible for dear-imgui global configurations.
// Soultatos Stefanos 2022

#ifndef GUI_CORE_HPP
#define GUI_CORE_HPP

#include <string>

// NOTE: Implemented as a singleton because:
//  i)  Dear-ImGui holds a global state.
//  ii) Must implement a ui component.

namespace gui
{

/***********************************************************
 * Core Config Data                                        *
 ***********************************************************/

struct core_config
{
    std::string color_theme;
    std::string font_name;
    int frame_rounding;
    bool window_bordered;
    bool frame_bordered;
    bool popup_bordered;

    auto operator==(const core_config&) const -> bool = default;
    auto operator!=(const core_config&) const -> bool = default;
};

/***********************************************************
 * Core Config API                                         *
 ***********************************************************/

class core_config_api
{
public:
    using config_data_type = core_config;

    explicit core_config_api(config_data_type cfg);

    auto config_data() const -> const config_data_type& { return m_cfg; }

    auto set_color_theme(std::string theme) -> void;
    auto set_font_name(std::string name) -> void;
    auto set_frame_rounding(int frame_rounding) -> void;
    auto set_window_bordered(bool toggle) -> void;
    auto set_frame_bordered(bool toggle) -> void;
    auto set_popup_bordered(bool toggle) -> void;

private:
    config_data_type m_cfg;
};

/***********************************************************
 * Core                                                    *
 ***********************************************************/

// Provides a config interface fot the entire gui.
class core final
{
public:
    using config_data_type = core_config;
    using config_api_type = core_config_api;

    core(const core&) = delete;
    core(core&&) = delete;

    auto operator=(const core&) -> core& = delete;
    auto operator=(core&&) -> core& = delete;

    static auto is_initialized() -> bool;
    static auto initialize(config_data_type cfg) -> void;

    // Precondition: (gui::core::is_initialized() == true);
    static auto get() -> core&;

    auto default_data() const -> const config_data_type& { return m_defaults; }

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto config_api() const -> const config_api_type& { return m_api; }
    auto config_api() -> config_api_type& { return m_api; }

    auto draw(const config_data_type& cfg) const -> void;

private:
    explicit core(config_data_type cfg = config_data_type());
    ~core() = default;

    config_data_type m_cfg, m_defaults;
    config_api_type m_api;
};

} // namespace gui

#endif // GUI_CORE_HPP
