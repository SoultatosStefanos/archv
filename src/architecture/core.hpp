// Contains an entry point for the architecture package.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_CORE_HPP
#define ARCHITECTURE_CORE_HPP

#include "graph.hpp"
#include "symbol_table.hpp"

namespace architecture
{

// Could pull multiple initialization methods here if needed..

class core
{
public:
    explicit core(symbol_table st = symbol_table(), graph g = graph())
    : m_st { std::move(st) }, m_g { std::move(g) }
    {
    }

    auto get_symbol_table() const -> const symbol_table& { return m_st; }
    auto get_graph() const -> const graph& { return m_g; }

private:
    symbol_table m_st;
    graph m_g;
};

} // namespace architecture

#endif // ARCHITECTURE_CORE_HPP
