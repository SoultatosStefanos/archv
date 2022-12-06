// Contains a configuration module for the rendering package.
// Soultatos Stefanos 2022

#ifndef RENDERING_CONFIG_HPP
#define RENDERING_CONFIG_HPP

#include "background_config.hpp"     // for background_config
#include "degrees_ranked_config.hpp" // for degrees_ranked_config
#include "graph_config.hpp"          // for graph_config
#include "minimap_config.hpp"        // for minimap_config

namespace Json
{
class Value;
} // namespace Json

namespace rendering
{

/***********************************************************
 * Config data                                             *
 ***********************************************************/

struct config_data
{
    background_config background;
    graph_config graph;
    degrees_ranked_config degrees;
    minimap_config minimap;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

} // namespace rendering

#endif // RENDERING_CONFIG_HPP
