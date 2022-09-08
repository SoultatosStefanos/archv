// Contains a module for managing topology types via ids at run/compile time.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_PLUGIN_HPP
#define LAYOUT_TOPOLOGY_PLUGIN_HPP

#include <array>
#include <string>

namespace layout
{

class topology;

namespace topology_plugin
{
    using identifier = std::string;

    static constexpr auto cube_id = "Cube";
    static constexpr auto sphere_id = "Sphere";

    static constexpr auto enumeration = std::array { cube_id, sphere_id };

    constexpr auto enumerates(const identifier& tag) -> bool
    {
        constexpr auto& set = enumeration;
        return std::find(std::begin(set), std::end(set), tag) != std::end(set);
    }

    auto identify(const topology&) -> identifier;

} // namespace topology_plugin

} // namespace layout

#endif // LAYOUT_TOPOLOGY_PLUGIN_HPP
