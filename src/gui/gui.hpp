// Contains a class responsible for dear-imgui global configurations.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

#include "ui/ui_component.hpp"

#include <string>

namespace gui
{

/***********************************************************
 * GUI Config Data                                         *
 ***********************************************************/

struct gui_config
{
    std::string color_theme;
    std::string font_name;
    int frame_rounding;
    bool window_bordered;
    bool frame_bordered;
    bool popup_bordered;

    auto operator==(const gui_config&) const -> bool = default;
    auto operator!=(const gui_config&) const -> bool = default;
};

/***********************************************************
 * GUI Config API                                          *
 ***********************************************************/

class gui_config_api
{
public:
    using config_data_type = gui_config;

    explicit gui_config_api(config_data_type cfg);

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
 * GUI                                                     *
 ***********************************************************/

// NOTE: Implemented as a singleton because:
//  i)  Dear-ImGui holds a global state.
//  ii) Must implement a ui component.

class gui final : public ui::ui_component< gui_config, gui_config_api >
{
public:
    using base = ui::ui_component< gui_config, gui_config_api >;
    using config_data_type = typename base::config_data_type;
    using config_api_type = typename base::config_api_type;

    gui(const gui&) = delete;
    gui(gui&&) = delete;

    auto operator=(const gui&) -> gui& = delete;
    auto operator=(gui&&) -> gui& = delete;

    static auto get(const config_data_type& cfg) -> gui&
    {
        static auto singleton = gui(cfg);
        return singleton;
    }

    // Call once at start.
    auto draw(const config_data_type& cfg) const -> void override final;

private:
    explicit gui(config_data_type cfg = config_data_type());
    ~gui() override = default;
};

} // namespace gui

#endif // GUI_GUI_HPP
