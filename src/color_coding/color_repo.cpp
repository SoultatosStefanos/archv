#include "color_repo.hpp"

#include <cassert>

namespace color_coding
{

color_repo::color_repo(hash_table map) : m_map { std::move(map) }
{
}

auto color_repo::get_color(dependency_type dep) const -> color_code
{
    const auto iter = m_map.find(dep); // heterogeneous, no allocations
    assert(iter != std::cend(m_map));
    return (*iter).second;
}

auto color_repo::set_color(dependency_type dep, color_code rgba) -> void
{
    auto iter = m_map.find(dep);
    assert(iter != std::cend(m_map));
    (*iter).second = rgba; // heterogeneous, no allocations
    assert(get_color(dep) == rgba);
}

} // namespace color_coding