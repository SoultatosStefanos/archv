// Contains a gui package configuration module.
// Soultatos Stefanos 2022

#ifndef GUI_CONFIG_HPP
#define GUI_CONFIG_HPP

#include "gui.hpp"

#include <jsoncpp/json/json.h>

namespace gui
{

using config_data = gui::config_data_type;

auto default_config() -> config_data;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace gui

#endif // GUI_CONFIG_HPP
