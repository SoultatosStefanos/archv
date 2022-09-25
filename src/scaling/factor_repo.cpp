#include "factor_repo.hpp"

#include <cassert>

namespace scaling
{

factor_repo::factor_repo(hash_table factors) : m_map { std::move(factors) }
{
}

auto factor_repo::get_factor(tag_type tag) const -> const factor&
{
    const auto iter = m_map.find(tag); // heterogeneous
    assert(iter != std::cend(m_map));
    return (*iter).second;
}

auto factor_repo::get_factor(tag_type tag) -> factor&
{
    return const_cast< factor& >(std::as_const(*this).get_factor(tag));
}

} // namespace scaling