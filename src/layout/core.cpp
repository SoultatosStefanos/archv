#include "core.hpp"

#include "cube.hpp"
#include "sphere.hpp"

namespace layout::detail
{

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    if (desc == topology_traits<cube>::desc())
    {
        return std::make_unique<cube>(scale);
    }
    else if (desc == topology_traits<sphere>::desc())
    {
        return std::make_unique<sphere>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

/***********************************************************
 * Revert to defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_command : public utility::command
{
public:
    using signal = revert_to_defaults_service::signal;
    using graph = revert_to_defaults_service::graph;
    using weight_map = revert_to_defaults_service::weight_map;
    using command_history = revert_to_defaults_service::command_history;
    using layout_pointer = revert_to_defaults_service::layout_pointer;
    using topology_pointer = revert_to_defaults_service::topology_pointer;
    using layout_descriptor = revert_to_defaults_service::layout_descriptor;
    using topology_descriptor = revert_to_defaults_service::topology_descriptor;
    using topology_scale = revert_to_defaults_service::topology_scale;

    revert_to_defaults_command(signal& s,
                               const graph& g,
                               weight_map edge_weight,
                               layout_descriptor layout_desc,
                               topology_descriptor topology_desc,
                               topology_scale topology_scale,
                               layout_pointer& layout,
                               topology_pointer& space);

    virtual ~revert_to_defaults_command() override = default;

    virtual void execute() override;
    virtual void undo() override;
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<revert_to_defaults_command>(*this);
    }

private:
    void change_layout(const layout_descriptor& layout_desc,
                       const topology_descriptor& topology_desc,
                       topology_scale topology_scale);

    signal& m_signal;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_descriptor m_layout_desc;
    layout_descriptor m_prev_layout_desc;
    topology_descriptor m_topology_desc;
    topology_descriptor m_prev_topology_desc;
    topology_scale m_topology_scale;
    topology_scale m_prev_topology_scale;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

revert_to_defaults_command::revert_to_defaults_command(
    signal& s,
    const graph& g,
    weight_map edge_weight,
    layout_descriptor layout_desc,
    topology_descriptor topology_desc,
    topology_scale topology_scale,
    layout_pointer& layout,
    topology_pointer& space)
    : m_signal{s},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout_desc{layout_desc},
      m_prev_layout_desc{layout->desc()},
      m_topology_desc{topology_desc},
      m_prev_topology_desc{space->desc()},
      m_topology_scale{topology_scale},
      m_prev_topology_scale{space->scale()},
      m_layout{layout},
      m_topology{space}
{}

void revert_to_defaults_command::execute()
{
    if (m_layout_desc != m_layout->desc() or
        m_topology_desc != m_topology->desc() or
        m_topology_scale != m_topology->scale())
        change_layout(m_layout_desc, m_topology_desc, m_topology_scale);
}

void revert_to_defaults_command::undo()
{
    if (m_prev_layout_desc != m_layout->desc() or
        m_prev_topology_desc != m_topology->desc() or
        m_prev_topology_scale != m_topology->scale())
        change_layout(
            m_prev_layout_desc, m_prev_topology_desc, m_prev_topology_scale);
}

void revert_to_defaults_command::change_layout(
    const layout_descriptor& layout_desc,
    const topology_descriptor& topology_desc,
    topology_scale topology_scale)
{
    m_topology = topology_factory::make_topology(topology_desc, topology_scale);
    m_layout = layout_factory<graph>::make_layout(
        layout_desc, m_g, *m_topology, m_edge_weight);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_desc
                            << " with scale: " << topology_scale;
    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << layout_desc;

    m_signal(*m_layout, *m_topology);
}

revert_to_defaults_service::revert_to_defaults_service(command_history& cmds,
                                                       const graph& g,
                                                       weight_map edge_weight,
                                                       layout_pointer& layout,
                                                       topology_pointer& space)
    : m_cmds{cmds},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout_desc{layout->desc()},
      m_topology_desc{space->desc()},
      m_topology_scale{space->scale()},
      m_layout{layout},
      m_topology{space}
{}

void revert_to_defaults_service::operator()()
{
    m_cmds.execute(
        std::make_unique<revert_to_defaults_command>(m_signal,
                                                     m_g,
                                                     m_edge_weight,
                                                     m_layout_desc,
                                                     m_topology_desc,
                                                     m_topology_scale,
                                                     m_layout,
                                                     m_topology));
}

} // namespace layout::detail

namespace layout
{

core::core(command_history& cmds,
           const graph& g,
           weight_map edge_weight,
           const std::string& layout_type,
           const std::string& topolgy_type,
           double topology_scale)
    : m_topology{detail::topology_factory::make_topology(topolgy_type,
                                                         topology_scale)},
      m_layout{detail::layout_factory<graph>::make_layout(
          layout_type, g, *m_topology, edge_weight)},
      m_update_layout{cmds, g, edge_weight, m_layout, m_topology},
      m_update_topology{cmds, g, edge_weight, m_layout, m_topology},
      m_revert_to_defaults{cmds, g, edge_weight, m_layout, m_topology}
{
    m_update_layout.connect([this](const auto& l) { m_layout_signal(l); });

    m_update_topology.connect([this](const auto& l, const auto& s) {
        m_layout_signal(l);
        m_topology_signal(s);
    });

    m_revert_to_defaults.connect([this](const auto& l, const auto& s) {
        m_layout_signal(l);
        m_topology_signal(s);
    });
}

void core::update_layout(const std::string& type)
{
    m_update_layout(type);
}

void core::update_topology(const std::string& type, double scale)
{
    m_update_topology(type, scale);
}

void core::revert_to_defaults()
{
    m_revert_to_defaults();
}

} // namespace layout