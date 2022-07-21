#include "symbol_table.hpp"

namespace symbols
{

auto symbol_table::lookup(const id_type& id) const -> const structure*
{
    return m_table.contains(id) ? &m_table.at(id) : nullptr;
}

void symbol_table::insert(structure s)
{
    m_table.insert(std::make_pair(s.sym.id, std::move(s)));
}

} // namespace symbols