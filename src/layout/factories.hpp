// Contains the factories of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_FACTORIES_HPP
#define LAYOUT_FACTORIES_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <memory>

namespace layout
{

class layout_factory final // TODO Implement caching
{
public:
    using graph = architecture::graph;
    using pointer = std::unique_ptr<layout>;

    enum class type_name
    {
        gursoy_atun = 0
    };

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    static auto resolve_type(const layout& l) -> type_name;

    static auto make_layout(type_name type,
                            const graph& g,
                            const topology& space) -> pointer;

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using scale_type = typename topology::scale_type;

    enum class type_name
    {
        cube = 0,
        sphere
    };

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto resolve_type(const topology& s) -> type_name;

    static auto make_topology(type_name type, scale_type scale) -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace layout

#endif // LAYOUT_FACTORIES_HPP
