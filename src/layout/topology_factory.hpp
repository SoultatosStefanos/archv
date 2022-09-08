// Contains a factory class for creating topology implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_FACTORY_HPP
#define LAYOUT_TOPOLOGY_FACTORY_HPP

#include "topology.hpp"

#include <algorithm>
#include <memory>
#include <string_view>

namespace layout
{

/***********************************************************
 * Topology Enumerator                                     *
 ***********************************************************/

// Helper struct to list the avalilable plugin types.
struct topology_enumerator final
{
    static constexpr auto cube_desc = "Cube";
    static constexpr auto sphere_desc = "Sphere";

    static constexpr auto enumeration = std::array { cube_desc, sphere_desc };

    topology_enumerator() = delete;
    topology_enumerator(const topology_enumerator&) = delete;
    topology_enumerator(topology_enumerator&&) = delete;
    ~topology_enumerator() = delete;

    auto operator=(const topology_enumerator&) -> topology_enumerator& = delete;
    auto operator=(topology_enumerator&&) -> topology_enumerator& = delete;
};

constexpr auto is_topology_enumerated(std::string_view desc) -> bool
{
    constexpr auto& set = topology_enumerator::enumeration;
    return std::find(std::begin(set), std::end(set), desc) != std::end(set);
}

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

class topology_factory final
{
public:
    using pointer = std::unique_ptr< topology >;
    using scale_type = topology::scale_type;
    using descriptor = topology::descriptor;
    using enumerator = topology_enumerator;

    topology_factory(const topology_factory&) = delete;
    topology_factory(topology_factory&&) = delete;

    auto operator=(const topology_factory&) -> topology_factory& = delete;
    auto operator=(topology_factory&&) -> topology_factory& = delete;

    static auto make_topology(const descriptor& d, scale_type s) -> pointer;

private:
    static auto make_topology_impl(const descriptor&, scale_type) -> pointer;

    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP
