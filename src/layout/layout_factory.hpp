// Contains a layout factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <memory>
#include <string>

namespace layout
{

class layout_factory final
{
public:
    using graph = architecture::graph;
    using pointer = std::unique_ptr<layout>;
    using layout_descriptor = typename layout_traits<layout>::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    static auto make_layout(layout_descriptor desc,
                            const graph& g,
                            const topology& space) -> pointer;

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_FACTORY_HPP
