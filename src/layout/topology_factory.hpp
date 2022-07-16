// Contains a topology factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_FACTORY_HPP
#define LAYOUT_TOPOLOGY_FACTORY_HPP

#include "topology.hpp"

#include <memory>
#include <string>

namespace layout
{

class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using topology_descriptor = typename topology_traits<topology>::descriptor;
    using topology_scale = typename topology_traits<topology>::scale_type;

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto make_topology(topology_descriptor desc, topology_scale scale)
        -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP
