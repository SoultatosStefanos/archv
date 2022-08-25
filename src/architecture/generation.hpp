// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GENERATION_HPP
#define ARCHITECTURE_GENERATION_HPP

#include "graph.hpp"
#include "progress/task.hpp"
#include "symbol_table.hpp"

#include <atomic>
#include <boost/signals2/signal.hpp>
#include <jsoncpp/json/json.h>
#include <unordered_map>

namespace architecture
{

class symbol_table;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
class generation : public progress::task
{
    using signal = boost::signals2::signal< void(const generation&) >;

public:
    using vertex_property = graph::vertex_bundled;
    using vertex = graph::vertex_descriptor;
    using vertex_properties = std::unordered_map< vertex_property, vertex >;
    using result_type = std::tuple< symbol_table, graph, vertex_properties >;

    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    explicit generation(const Json::Value& root);
    virtual ~generation() override = default;

    virtual auto total_units() const -> units override;
    virtual auto units_done() const -> units override;

    virtual auto stop() -> void override;
    virtual auto suspend() -> void override;
    virtual auto resume() -> void override;

    virtual auto work(units todo) -> void override;

    auto connect(const slot& monitor) -> connection
    {
        return m_signal.connect(monitor);
    }

    // Invalidates this object's state.
    auto result() const -> result_type
    {
#ifndef NDEBUG
        m_done = true;
#endif
        return { std::move(m_st), boost::move(m_g), std::move(m_properties) };
    }

protected:
    auto emit_status() const -> void { m_signal(*this); }

    auto dispatch_progress() -> void;
    auto deserialize_structure() -> void;
    auto deserialize_dependency() -> void;

private:
    signal m_signal;

    const Json::Value& m_structs_root;
    const Json::Value& m_deps_root;

    symbol_table m_st;
    graph m_g;
    vertex_properties m_properties;

    Json::Value::const_iterator m_structs_curr;
    Json::Value::const_iterator m_deps_curr;
    std::atomic_bool m_greenlit { true };

    mutable bool m_done { false };
};

// Convenience function for unmonitored architecture generation.
inline auto generate_arch(const Json::Value& root) -> generation::result_type
{
    auto task = generation(root);
    progress::work_all_units_at_once(task);
    return task.result();
}

} // namespace architecture

#endif // ARCHITECTURE_GENERATION_HPP
