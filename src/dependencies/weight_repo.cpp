#include "weight_repo.hpp"

#include <cassert>

namespace dependencies
{

weight_repo::weight_repo(hash_table table) : m_map { std::move(table) } { }

auto weight_repo::get_weight(const dependency_type& type) const -> weight_type
{
    assert(m_map.contains(type));
    return m_map.at(type);
}

auto weight_repo::set_weight(const dependency_type& type, weight_type w) -> void
{
    m_map[type] = w;
    assert(get_weight(type) == w);
}

} // namespace dependencies