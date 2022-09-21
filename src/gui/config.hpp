// Contains a gui package configuration module.
// Soultatos Stefanos 2022

#ifndef GUI_CONFIG_HPP
#define GUI_CONFIG_HPP

#include "gui.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace gui
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct deserialization_error : virtual std::exception, virtual boost::exception
{
};

struct unknown_color_theme : virtual deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using color_theme_info = boost::error_info< struct tag_theme, std::string >;

/***********************************************************
 * Functions                                               *
 ***********************************************************/

using config_data = gui::config_data_type;

auto default_config() -> config_data;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace gui

#endif // GUI_CONFIG_HPP
