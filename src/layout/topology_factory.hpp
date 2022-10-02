// Contains a factory class for creating topology implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_FACTORY_HPP
#define LAYOUT_TOPOLOGY_FACTORY_HPP

#include "topology.hpp"
#include "topology_plugin.hpp"

#include <memory>

namespace layout
{

class topology_factory final
{
public:
    using pointer = std::unique_ptr< topology >;
    using id_type = topology_id;
    using scale_type = topology::scale_type;

    topology_factory(const topology_factory&) = delete;
    topology_factory(topology_factory&&) = delete;

    auto operator=(const topology_factory&) -> topology_factory& = delete;
    auto operator=(topology_factory&&) -> topology_factory& = delete;

    static auto make_topology(id_type id, scale_type s) -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP