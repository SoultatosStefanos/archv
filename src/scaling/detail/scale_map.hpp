// Contains a private module for the scaling/scale_map header.
// Soultatos Stefanos 2022

#ifndef SCALING_DETAIL_SCALE_MAP_HPP
#define SCALING_DETAIL_SCALE_MAP_HPP

#include "scaling/backend.hpp"
#include "scaling/scale.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <cassert>
#include <numeric>

namespace scaling::detail
{

template < typename Graph, typename FactorCounter >
class scale_dispatcher
{
    using graph_traits = boost::graph_traits< Graph >;

public:
    using graph_type = Graph;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using factor_counter_type = FactorCounter;

    scale_dispatcher(const backend& b, factor_counter_type f)
    : m_backend { &b }, m_count_factors { std::move(f) }
    {
        assert(m_backend);
    }

    auto operator()(vertex_type v) const -> scale_vector
    {
        const auto& repo = *(m_backend->get_factor_repo());
        return std::accumulate(
            std::cbegin(repo),
            std::cend(repo),
            make_neutral_scale(),
            [this, v](const auto& lhs, const auto& rhs)
            {
                const auto& tag = rhs.first;
                const auto& factor = rhs.second;
                const auto num = m_count_factors(v, tag);

                static_assert(std::is_integral_v< decltype(num) >);

                return combine(lhs, scale(factor, num));
            });
    }

private:
    const backend* m_backend { nullptr };
    factor_counter_type m_count_factors;
};

} // namespace scaling::detail

#endif // SCALING_DETAIL_SCALE_MAP_HPP
