#include "weight_repo.hpp"

#include <cassert>

namespace dependencies
{

weight_repo::weight_repo(hash_table table) : m_map{std::move(table)} {}

auto weight_repo::get_weight(const dependency_type& type) const -> weight
{
    assert(m_map.contains(type));
    return m_map.at(type);
}

void weight_repo::set_weight(const dependency_type& type, weight score)
{
    m_map[type] = score;
    assert(get_weight(type) == score);
}

} // namespace dependencies