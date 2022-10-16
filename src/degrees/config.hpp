// Configuration module for the degrees visualization subsystem.
// Soultatos Stefanos 2022

#ifndef DEGREES_CONFIG_HPP
#define DEGREES_CONFIG_HPP

#include "backend.hpp"

#include <jsoncpp/json/json.h>

namespace degrees
{

using config_data = backend::config_data_type;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace degrees

#endif // DEGREES_CONFIG_HPP
