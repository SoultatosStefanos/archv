// Contains a configuration module for the color_coding package.
// Soultatos Stefanos 2022

#ifndef COLOR_CODING_CONFIG_HPP
#define COLOR_CODING_CONFIG_HPP

#include "backend.hpp"         // for backend::config_data_type
#include "misc/jsoncppfwd.hpp" // for Value

namespace color_coding
{

using json_val = Json::Value;
using config_data = backend::config_data_type;

auto deserialize(const json_val& root) -> config_data;
auto serialize(json_val& root, const config_data& cfg) -> void;

} // namespace color_coding

#endif // COLOR_CODING_CONFIG_HPP
