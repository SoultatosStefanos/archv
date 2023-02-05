// Contains a configuration module for the rendering package.
// Soultatos Stefanos 2022

#ifndef RENDERING_CONFIG_HPP
#define RENDERING_CONFIG_HPP

#include "background_config.hpp" // for background_config
#include "graph_config.hpp"      // for graph_config
#include "minimap_config.hpp"    // for minimap_config
#include "misc/jsoncppfwd.hpp"   // for Value

namespace rendering
{

struct config_data
{
    background_config background;
    graph_config graph;
    minimap_config minimap;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

using json_val = Json::Value;

auto deserialize_background(const json_val& root) -> background_config;
auto deserialize_graph(const json_val& root) -> graph_config;
auto deserialize_minimap(const json_val& root) -> minimap_config;
auto deserialize(const json_val& root) -> config_data;

} // namespace rendering

#endif // RENDERING_CONFIG_HPP
