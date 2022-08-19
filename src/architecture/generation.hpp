// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GENERATION_HPP
#define ARCHITECTURE_GENERATION_HPP

#include "graph.hpp"
#include "progress/task.hpp"
#include "symbol_table.hpp"

#include <atomic>
#include <jsoncpp/json/json.h>
#include <unordered_map>

namespace architecture
{

class symbol_table;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
class generation : public progress::task
{
public:
    using vertex_property = graph::vertex_bundled;
    using vertex = graph::vertex_descriptor;
    using vertex_properties = std::unordered_map< vertex_property, vertex >;

    generation(
        const Json::Value& root,
        symbol_table& st,
        graph& g,
        vertex_properties& properties);

    virtual ~generation() override = default;

    virtual auto total_units() const -> units override;
    virtual auto units_done() const -> units override;

    virtual auto stop() -> void override;
    virtual auto suspend() -> void override;
    virtual auto resume() -> void override;

    virtual auto work(units todo) -> void override;

protected:
    auto dispatch_progress() -> void;
    auto deserialize_structure() -> void;
    auto deserialize_dependency() -> void;

private:
    const Json::Value& m_structs_root;
    const Json::Value& m_deps_root;

    symbol_table& m_st;
    graph& m_g;
    vertex_properties& m_properties;

    Json::Value::const_iterator m_structs_curr;
    Json::Value::const_iterator m_deps_curr;
    std::atomic_bool m_greenlit { true };
};

// Convenience function for unmonitored architecture generation.
inline auto generate_arch(const Json::Value& root)
{
    auto st = symbol_table();
    auto g = graph();
    auto props = generation::vertex_properties();

    auto task = generation(root, st, g, props);
    progress::work_all_units_at_once(task);

    return std::make_tuple(std::move(st), std::move(g), std::move(props));
}

} // namespace architecture

#endif // ARCHITECTURE_GENERATION_HPP
