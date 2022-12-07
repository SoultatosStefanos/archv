// Contains a gui package configuration module.
// Soultatos Stefanos 2022

#ifndef GUI_CONFIG_HPP
#define GUI_CONFIG_HPP

#include "misc/jsoncppfwd.hpp" // for Value

namespace gui
{

class gui_config;

using json_val = Json::Value;
using config_data = gui_config;

auto deserialize(const json_val& root) -> config_data;

} // namespace gui

#endif // GUI_CONFIG_HPP
