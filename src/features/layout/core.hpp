// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "factories.hpp"
#include "layout.hpp"
#include "services.hpp"
#include "topology.hpp"

#include <cassert>
#include <memory>

namespace features::layout
{

// A facade for the layout management subsystem.
class core final
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using command_history = utility::command_history;
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using slot_type = signal::slot_type;

    core(const core&) = delete;
    core(core&&) = delete;

    auto operator=(const core&) -> core& = delete;
    auto operator=(core&&) -> core& = delete;

    static auto get() -> core&
    {
        static core singleton;
        return singleton;
    }

    void initialize(command_history& cmds,
                    const graph& g,
                    weight_map edge_weight,
                    const std::string& layout_type,
                    const std::string& topolgy_type,
                    double topology_scale);
    void reset();

    auto get_layout_type() const -> std::string;
    auto get_topology_type() const -> std::string;
    auto get_topology_scale() const -> double;

    void update_layout(const std::string& type);
    void update_topology(const std::string& type, double scale);
    void revert_to_defaults();

    void connect(const slot_type& slot);

private:
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;

    core() = default;
    ~core() = default;

    layout_pointer m_layout;
    topology_pointer m_topology;

    std::unique_ptr<update_layout_service> m_update_layout;
    std::unique_ptr<update_topology_service> m_update_topology;
    std::unique_ptr<revert_to_defaults_service> m_revert_to_defaults;
};

} // namespace features::layout

#endif // LAYOUT_CORE_HPP
