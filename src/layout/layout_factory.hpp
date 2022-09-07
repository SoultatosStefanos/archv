// Contains a factory class for creating layout implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "layout_enumerator.hpp"
#include "topology.hpp"

#include <array>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <memory>

namespace layout
{

template < typename Graph >
class layout_factory final
{
public:
    using graph_type = Graph;
    using pointer = std::unique_ptr< layout< graph_type > >;
    using descriptor = typename layout< graph_type >::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    template < typename WeightMap >
    static auto make_layout(
        const descriptor& desc,
        const graph_type& g,
        const topology& space,
        WeightMap edge_weight) -> pointer
    {
        auto ptr = make_layout_impl(desc, g, space, edge_weight);
        assert(ptr->desc() == desc);
        assert(layout_enumerator::enumerates(ptr->desc()));
        return ptr;
    }

private:
    template < typename WeightMap >
    static auto make_layout_impl(
        const descriptor& desc,
        const graph_type& g,
        const topology& space,
        WeightMap edge_weight) -> pointer
    {
        if (desc == layout_enumerator::gursoy_atun_type)
        {
            return std::make_unique< gursoy_atun_layout< graph_type > >(
                g, space, edge_weight);
        }
        else
        {
            assert(!layout_enumerator::enumerates(desc));
            BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
            assert(false);
            return nullptr;
        }
    }

    layout_factory() = default;
    ~layout_factory() = default;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_FACTORY_HPP
