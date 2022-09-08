// Contains a configuration module for the rendering package.
// Soultatos Stefanos 2022

#ifndef RENDERING_CONFIG_HPP
#define RENDERING_CONFIG_HPP

#include "background_renderer.hpp"
#include "graph_renderer.hpp"

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

    // ...

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

} // namespace rendering

#endif // RENDERING_CONFIG_HPP
