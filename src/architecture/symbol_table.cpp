#include "symbol_table.hpp"

namespace architecture
{

auto symbol_table::lookup(const key_type& id) const -> const value_type*
{
    return m_table.contains(id) ? std::addressof(m_table.at(id)) : nullptr;
}

auto symbol_table::insert(value_type s) -> void
{
    m_table.insert(std::make_pair(s.sym.id, std::move(s)));
}

auto symbol_table::clear() -> void
{
    m_table.clear();
}

} // namespace architecture