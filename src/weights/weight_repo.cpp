#include "weight_repo.hpp"

#include <cassert>

namespace weights
{

weight_repo::weight_repo(hash_table table) : m_map { std::move(table) }
{
}

auto weight_repo::get_weight(dependency_type dependency) const -> weight_type
{
    const auto iter = m_map.find(dependency); // heterogeneous, no allocations
    assert(iter != std::cend(m_map));
    return (*iter).second;
}

auto weight_repo::set_weight(dependency_type dependency, weight_type w) -> void
{
    auto iter = m_map.find(dependency);
    assert(iter != std::cend(m_map));
    (*iter).second = w; // heterogeneous, no allocations
    assert(get_weight(dependency) == w);
}

} // namespace weights