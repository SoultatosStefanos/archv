// Contains an adaptor that bridges the gui configurations with the ui package
// abstract concepts. (See ui/concepts)
// Soultatos Stefanos 2022

#ifndef GUI_UI_ADAPTOR_HPP
#define GUI_UI_ADAPTOR_HPP

#include <boost/exception/all.hpp> // for exception
#include <stdexcept>               // for exception

namespace gui
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct gui_config_error : virtual std::exception, virtual boost::exception
{
};

struct unknown_color_theme : virtual gui_config_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using color_theme_info = boost::error_info< struct tag_theme, std::string >;

/***********************************************************
 * Global configs                                          *
 ***********************************************************/

class gui_config;
class gui_config_api;

auto set_configs(gui_config cfg) -> void;

auto get_default_configs() -> gui_config&;
auto get_configs() -> gui_config&;
auto get_config_api() -> gui_config_api&;

auto draw_configs(const gui_config& cfg) -> void;

/***********************************************************
 * UI Adaptor                                              *
 ***********************************************************/

// Bridges the gui configurations with the ui package abstract concepts.
// (See ui/concepts)
// NOTE: stateless.
struct ui_adaptor
{
    using config_data_type = gui_config;
    using config_api_type = gui_config_api;

    static auto default_data() -> const auto& { return get_default_configs(); }
    static auto config_data() -> auto& { return get_configs(); }
    static auto config_api() -> auto& { return get_config_api(); }

    static auto draw(const config_data_type& cfg) { draw_configs(cfg); }
};

} // namespace gui

#endif // GUI_UI_ADAPTOR_HPP
