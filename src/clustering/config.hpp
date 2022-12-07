// Contains a configuration module for the clustering package.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CONFIG_HPP
#define CLUSTERING_CONFIG_HPP

#include "misc/jsoncppfwd.hpp" // for Value

namespace clustering
{

class backend_config;

using json_val = Json::Value;
using config_data = backend_config;

auto deserialize(const json_val& root) -> config_data;

} // namespace clustering

#endif // CLUSTERING_CONFIG_HPP
