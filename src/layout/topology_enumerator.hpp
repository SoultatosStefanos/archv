// Contains a topology enumerator for the layout plugin system.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_ENUMERATOR_HPP
#define LAYOUT_TOPOLOGY_ENUMERATOR_HPP

#include <string_view>

namespace layout
{

namespace topology_enumerator
{
    static constexpr auto cube_type = "Cube";
    static constexpr auto sphere_type = "Sphere";

    constexpr auto enumerates(std::string_view type) -> bool
    {
        return type == cube_type or type == sphere_type /* or .... */;
    }

} // namespace topology_enumerator

} // namespace layout

#endif // LAYOUT_TOPOLOGY_ENUMERATOR_HPP
