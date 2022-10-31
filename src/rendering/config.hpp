// Contains a configuration module for the rendering package.
// Soultatos Stefanos 2022

#ifndef RENDERING_CONFIG_HPP
#define RENDERING_CONFIG_HPP

#include "background_config.hpp"
#include "degrees_ranked_config.hpp"
#include "graph_config.hpp"

#include <jsoncpp/json/json.h>

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

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

} // namespace rendering

#endif // RENDERING_CONFIG_HPP
