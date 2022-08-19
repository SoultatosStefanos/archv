// Contains a configuration module for the architecture dependencies.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_CONFIG_HPP
#define DEPENDENCIES_CONFIG_HPP

#include "weight_repo.hpp"

#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_map>

namespace dependencies
{

using config_data = weight_repo::hash_table;

auto deserialize_dependencies(const Json::Value& root) -> config_data;

// TODO: serialize, defaults

} // namespace dependencies

#endif // DEPENDENCIES_CONFIG_HPP
