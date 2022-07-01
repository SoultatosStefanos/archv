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

    TopologyFactory(const TopologyFactory&) = delete;
    TopologyFactory(TopologyFactory&&) = delete;

    auto operator=(const TopologyFactory&) -> TopologyFactory& = delete;
    auto operator=(TopologyFactory&&) -> TopologyFactory& = delete;

    static auto make_topology(const TopologyType& type, double scale)
        -> Topology;

private:
    TopologyFactory() = default;
    ~TopologyFactory() = default;
};

} // namespace Layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP
