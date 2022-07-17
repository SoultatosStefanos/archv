// Contains a topology factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_FACTORY_HPP
#define LAYOUT_TOPOLOGY_FACTORY_HPP

#include "topology.hpp"

#include <memory>
#include <string_view>

namespace layout
{

// In respect to typeid() operator.
class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using type_name = std::string_view;
    using scale_type = typename topology::scale_type;

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto make_topology(type_name type, scale_type scale) -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_FACTORY_HPP
