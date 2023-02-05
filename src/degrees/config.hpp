// Contains a configuration module for the rendering package.
// Soultatos Stefanos 2022

#ifndef DEGREES_CONFIG_HPP
#define DEGREES_CONFIG_HPP

#include "misc/jsoncppfwd.hpp" // for Value

namespace degrees
{

struct backend_config;
using config_data = backend_config;
using json_val = Json::Value;

auto deserialize(const json_val& root) -> config_data;
auto serialize(json_val& root, const config_data& cfg) -> void;

} // namespace degrees

#endif // DEGREES_CONFIG_HPP
