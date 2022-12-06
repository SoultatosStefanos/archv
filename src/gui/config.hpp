// Contains a gui package configuration module.
// Soultatos Stefanos 2022

#ifndef GUI_CONFIG_HPP
#define GUI_CONFIG_HPP

namespace Json
{
class Value;
} // namespace Json

namespace gui
{

class gui_config;

using config_data = gui_config;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace gui

#endif // GUI_CONFIG_HPP
