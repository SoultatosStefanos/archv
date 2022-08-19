// Contains tools for deserializing the visualized architecture.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_DESERIALIZATION_HPP
#define ARCHITECTURE_DESERIALIZATION_HPP

#include "graph.hpp"
#include "progress/task.hpp"
#include "symbol_table.hpp"

#include <future>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <utility>

// NOTE: Assumes specific json formatting: (see data/tests/samples_graphs).

namespace architecture
{

class symbols_deserialization : public progress::task
{
public:
    symbols_deserialization(const Json::Value& root, symbol_table& st);
    virtual ~symbols_deserialization() override = default;

    virtual auto total_units() const -> units override;
    virtual auto units_done() const -> units override;

    virtual auto stop() -> void override;
    virtual auto suspend() -> void override;
    virtual auto resume() -> void override;

    virtual auto work(units todo) -> void override;

private:
    auto deserialize_one() -> void;

    const Json::Value& m_root;
    symbol_table& m_st;
    Json::Value::const_iterator m_curr;
    std::atomic< bool > m_greenlit { true };
};

// Convenience utility.
inline auto deserialize_symbols(const Json::Value& root) -> symbol_table
{
    auto result = symbol_table();
    auto task = symbols_deserialization(root, result);
    progress::work_all_units_at_once(task);
    return result;
}

// Keeps track of each vertex desciptor by structure id.
using vertex_id_cache
    = std::unordered_map< graph::vertex_bundled, graph::vertex_descriptor >;

auto deserialize_dependencies(const Json::Value& root, const symbol_table& st)
    -> std::pair< graph, vertex_id_cache >;

} // namespace architecture

#endif // ARCHITECTURE_DESERIALIZATION_HPP
