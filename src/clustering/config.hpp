// Contains a configuration module for the clustering package.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CONFIG_HPP
#define CLUSTERING_CONFIG_HPP

namespace Json
{
class Value;
} // namespace Json

namespace clustering
{

class backend_config;

using config_data = backend_config;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace clustering

#endif // CLUSTERING_CONFIG_HPP
