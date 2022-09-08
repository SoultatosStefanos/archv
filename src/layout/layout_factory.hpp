// Contains a factory class for creating layout implementations.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_FACTORY_HPP
#define LAYOUT_LAYOUT_FACTORY_HPP

#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <algorithm>
#include <array>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <memory>
#include <string_view>

namespace layout
{

/***********************************************************
 * Layout Enumerator                                       *
 ***********************************************************/

// Helper struct to list the avalilable plugin types.
struct layout_enumerator final
{
    static constexpr auto gursoy_atun_desc = "Gursoy Atun";

    static constexpr auto enumeration = std::array { gursoy_atun_desc };

    layout_enumerator() = delete;
    layout_enumerator(const layout_enumerator&) = delete;
    layout_enumerator(layout_enumerator&&) = delete;
    ~layout_enumerator() = delete;

    auto operator=(const layout_enumerator&) -> layout_enumerator& = delete;
    auto operator=(layout_enumerator&&) -> layout_enumerator& = delete;
};

constexpr auto is_layout_enumerated(std::string_view desc) -> bool
{
    constexpr auto& set = layout_enumerator::enumeration;
    return std::find(std::begin(set), std::end(set), desc) != std::end(set);
}

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

template < typename Graph >
class layout_factory final
{
public:
    using graph_type = Graph;
    using pointer = std::unique_ptr< layout< graph_type > >;
    using descriptor = typename layout< graph_type >::descriptor;
    using enumerator = layout_enumerator;

    static_assert(
        gursoy_atun_layout< graph_type >::description
        == enumerator::gursoy_atun_desc);

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
        assert(is_layout_enumerated(ptr->desc()));
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
        if (desc == enumerator::gursoy_atun_desc)
        {
            return std::make_unique< gursoy_atun_layout< graph_type > >(
                g, space, edge_weight);
        }
        else
        {
            assert(!is_layout_enumerated(desc));
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
