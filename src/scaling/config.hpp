// Contains a configuration module for the vertex dynamic scaling subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_CONFIG_HPP
#define SCALING_CONFIG_HPP

#include "backend.hpp"

#include <jsoncpp/json/json.h>

namespace scaling
{

using config_data = backend::config_data_type;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace scaling

#endif // SCALING_CONFIG_HPP
