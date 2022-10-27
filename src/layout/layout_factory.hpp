// Contains a factory class for creating layout implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "plugin.hpp"
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
    using graph_type = Graph;
    using id_type = id_t;
    using pointer = std::unique_ptr< layout< graph_type > >;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    template < typename WeightMap >
    static auto make_layout(
        id_type id,
        const graph_type& g,
        const topology& space,
        WeightMap edge_weight) -> pointer
    {
        using gursoy_atun_type = gursoy_atun_layout< graph_type >;

        if (id == gursoy_atun_id)
        {
            return std::make_unique< gursoy_atun_type >(g, space, edge_weight);
        }
        else
        {
            assert(!is_layout_plugged_in(id));
            BOOST_LOG_TRIVIAL(fatal) << "invalid layout id: " << id;
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
