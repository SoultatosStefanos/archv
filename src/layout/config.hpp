// Contains a configuration module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONFIG_HPP
#define LAYOUT_CONFIG_HPP

#include "misc/jsoncppfwd.hpp" // for Value

namespace layout
{

struct backend_config;

using json_val = Json::Value;
using config_data = backend_config;

auto deserialize(const json_val& root) -> config_data;

} // namespace layout

#endif // LAYOUT_CONFIG_HPP
