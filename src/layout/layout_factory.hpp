// Contains a layout factory.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <memory>
#include <string>

namespace Layout
{

class LayoutFactory
{
public:
    using LayoutType = std::string;
    using Graph = Architecture::Graph;
    using UniquePtr = std::unique_ptr<Layout>;

    static constexpr auto gursoy_atun{"gursoy_atun"};

    static_assert(std::is_constructible_v<LayoutType, decltype(gursoy_atun)>);

    LayoutFactory() = default;
    LayoutFactory(const LayoutFactory&) = default;
    LayoutFactory(LayoutFactory&&) = default;

    virtual ~LayoutFactory() = default;

    auto operator=(const LayoutFactory&) -> LayoutFactory& = default;
    auto operator=(LayoutFactory&&) -> LayoutFactory& = default;

    virtual auto make_layout(const LayoutType& type,
                             const Graph& g,
                             const Topology& space) const -> UniquePtr = 0;
};

} // namespace Layout

#endif // LAYOUT_LAYOUT_FACTORY_HPP
