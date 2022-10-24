// Contains the configuration data of the layout subsystem backend.
// SOultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_CONFIG_HPP
#define LAYOUT_BACKEND_CONFIG_HPP

#include <string>
#include <vector>

namespace layout
{

struct backend_config
{
    using id_type = std::string;
    using scale_type = double;

    std::vector< id_type > layouts;
    std::vector< id_type > topologies;

    id_type layout;
    id_type topology;
    scale_type scale;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

} // namespace layout

#endif // LAYOUT_BACKEND_CONFIG_HPP
