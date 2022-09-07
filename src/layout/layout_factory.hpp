// Contains a factory class for creating layout implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <memory>

namespace layout
{

template < typename Graph >
class layout_factory final
{
public:
    using graph = Graph;
    using pointer = std::unique_ptr< layout< graph > >;
    using descriptor = typename layout< graph >::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    template < typename WeightMap >
    static auto make_layout(
        const descriptor& desc,
        const graph& g,
        const topology& space,
        WeightMap edge_weight) -> pointer
    {
        if (desc == gursoy_atun_layout< graph >::description)
        {
            return std::make_unique< gursoy_atun_layout< graph > >(
                g, space, edge_weight);
        }
        else
        {
            BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
            assert(false);
            return nullptr;
        }
    }

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_FACTORY_HPP
