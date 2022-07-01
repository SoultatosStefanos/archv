#include "layout_factory.hpp"

namespace Layout
{

auto LayoutFactory::make_layout(const LayoutType& type,
                                const Graph& g,
                                const Topology& space) -> UniquePtr
{
    if (type == gursoy_atun)
    {
        return std::make_unique<GursoyAtunLayout>(g, space);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}

} // namespace Layout