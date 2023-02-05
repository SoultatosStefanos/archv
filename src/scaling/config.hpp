// Contains a configuration module for the vertex dynamic scaling subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_CONFIG_HPP
#define SCALING_CONFIG_HPP

#include "backend.hpp"         // for backend::config_data_type;
#include "misc/jsoncppfwd.hpp" // for Value

namespace scaling
{

using json_val = Json::Value;
using config_data = backend::config_data_type;

auto deserialize(const json_val& root) -> config_data;
auto serialize(json_val& root, const config_data& cfg) -> void;

} // namespace scaling

#endif // SCALING_CONFIG_HPP
