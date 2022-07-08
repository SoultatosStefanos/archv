// Contains a layout factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <memory>
#include <string>

namespace visualization::layout
{

class layout_factory final
{
public:
    using graph = architecture::graph;

    static constexpr auto gursoy_atun_type{"gursoy_atun"};

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    static auto make_layout(const std::string& type,
                            const graph& g,
                            const topology& space) -> std::unique_ptr<layout>;

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

} // namespace visualization::layout

#endif // LAYOUT_LAYOUT_FACTORY_HPP
