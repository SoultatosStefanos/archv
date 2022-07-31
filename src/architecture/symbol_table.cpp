#include "symbol_table.hpp"

namespace architecture
{

auto symbol_table::lookup(const key_type& id) const -> const value_type*
{
    return m_table.contains(id) ? std::addressof(m_table.at(id)) : nullptr;
}

void symbol_table::insert(value_type s)
{
    m_table.insert(std::make_pair(s.sym.id, std::move(s)));
}

} // namespace architecture