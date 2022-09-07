// Contains a configuration module for the architecture dependencies.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_CONFIG_HPP
#define DEPENDENCIES_CONFIG_HPP

#include "backend.hpp"

#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_map>

namespace dependencies
{

using config_data = backend::config_data_type;

auto deserialize(const Json::Value& root) -> config_data;

// TODO: serialize, defaults

} // namespace dependencies

#endif // DEPENDENCIES_CONFIG_HPP
