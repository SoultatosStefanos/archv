// Contains a configuration module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONFIG_HPP
#define LAYOUT_CONFIG_HPP

namespace Json
{
class Value;
} // namespace Json

namespace layout
{

struct backend_config;

using config_data = backend_config;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace layout

#endif // LAYOUT_CONFIG_HPP
