// Contains the symbol table for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef SYMBOLS_SYMBOL_TABLE_HPP
#define SYMBOLS_SYMBOL_TABLE_HPP

#include "symbols.hpp"

#include <unordered_map>

namespace symbols
{

class symbol_table
{
    using storage = std::unordered_map<symbol::id_type, structure>;

public:
    using id_type = symbol::id_type;
    using const_iterator = storage::const_iterator;

    auto lookup(const id_type& id) const -> const structure*;

    void insert(structure s);

    auto begin() const -> const_iterator { return std::begin(m_table); }
    auto cbegin() const -> const_iterator { return std::cbegin(m_table); }
    auto end() const -> const_iterator { return std::cend(m_table); }
    auto cend() const -> const_iterator { return std::end(m_table); }

    auto operator==(const symbol_table&) const -> bool = default;
    auto operator!=(const symbol_table&) const -> bool = default;

private:
    storage m_table;
};

} // namespace symbols

#endif // SYMBOLS_SYMBOL_TABLE_HPP
