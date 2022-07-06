// Contains a topology factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_FACTORY_HPP
#define LAYOUT_TOPOLOGY_FACTORY_HPP

#include "topology.hpp"

#include <string>

namespace Layout
{

class TopologyFactory
{
public:
    using TopologyType = std::string;

    static constexpr auto cube_type{"cube"};
    static constexpr auto sphere_type{"sphere"};

    static_assert(std::is_constructible_v<TopologyType, decltype(cube_type)>);
    static_assert(std::is_constructible_v<TopologyType, decltype(sphere_type)>);

    TopologyFactory() = default;
    TopologyFactory(const TopologyFactory&) = default;
    TopologyFactory(TopologyFactory&&) = default;
    virtual ~TopologyFactory() = default;

    auto operator=(const TopologyFactory&) -> TopologyFactory& = default;
    auto operator=(TopologyFactory&&) -> TopologyFactory& = default;

    virtual auto make_topology(const TopologyType& type, double scale) const
        -> Topology = 0;
};

} // namespace Layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP
