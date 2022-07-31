// Contains the symbol table for the visualized architecture components.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_SYMBOL_TABLE_HPP
#define ARCHITECTURE_SYMBOL_TABLE_HPP

#include "symbols.hpp"

#include <unordered_map>

namespace architecture
{

/***********************************************************
 * Symbol Table                                            *
 ***********************************************************/

class symbol_table
{
public:
    using key_type = structure::id_type;
    using value_type = structure;

    auto lookup(const key_type& id) const -> const value_type*;
    void insert(value_type s);

    auto begin() -> auto { return std::begin(m_table); }
    auto begin() const -> auto { return std::begin(m_table); }
    auto cbegin() const -> auto { return std::cbegin(m_table); }

    auto end() -> auto { return std::end(m_table); }
    auto end() const -> auto { return std::end(m_table); }
    auto cend() const -> auto { return std::cend(m_table); }

    auto operator==(const symbol_table&) const -> bool = default;
    auto operator!=(const symbol_table&) const -> bool = default;

private:
    using hash_table = std::unordered_map<key_type, value_type>;

    hash_table m_table;
};

} // namespace architecture

#endif // ARCHITECTURE_SYMBOL_TABLE_HPP
