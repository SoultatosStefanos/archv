#include "core.hpp"

#include "cube.hpp"
#include "gursoy_atun_layout.hpp"
#include "sphere.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace layout::detail
{

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

auto layout_factory::make_layout(const descriptor& desc,
                                 const graph& g,
                                 const topology& space,
                                 weight_map edge_weight) -> pointer
{
    if (desc == layout_traits<gursoy_atun_layout>::desc())
    {
        return std::make_unique<gursoy_atun_layout>(g, space, edge_weight);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
        assert(false);
        return nullptr;
    }
}

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
 * Update Layout Use Case                                  *
 ***********************************************************/

class update_layout_command : public utility::command
{
public:
    using signal = update_layout_service::signal;
    using graph = update_layout_service::graph;
    using weight_map = update_layout_service::weight_map;
    using command_history = update_layout_service::command_history;
    using layout_pointer = update_layout_service::layout_pointer;
    using topology_pointer = update_layout_service::topology_pointer;
    using descriptor = update_layout_service::descriptor;

    update_layout_command(signal& s,
                          descriptor desc,
                          const graph& g,
                          weight_map edge_weight,
                          layout_pointer& layout,
                          const topology_pointer& space);

    virtual ~update_layout_command() override = default;

    virtual void execute() override;
    virtual void undo() override;
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr<command> override;

private:
    void change_layout(const descriptor& desc);

    signal& m_signal;

    descriptor m_desc;
    descriptor m_prev_desc;

    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

update_layout_command::update_layout_command(signal& s,
                                             descriptor desc,
                                             const graph& g,
                                             weight_map edge_weight,
                                             layout_pointer& layout,
                                             const topology_pointer& space)
    : m_signal{s},
      m_desc{desc},
      m_prev_desc{layout->desc()},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_space{space}
{}

void update_layout_command::execute()
{
    if (m_desc != m_layout->desc())
        change_layout(m_desc);
}

void update_layout_command::undo()
{
    if (m_prev_desc != m_layout->desc())
        change_layout(m_prev_desc);
}

void update_layout_command::change_layout(const descriptor& desc)
{
    m_layout = layout_factory::make_layout(desc, m_g, *m_space, m_edge_weight);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << desc;

    m_signal(*m_layout);
}

auto update_layout_command::clone() const -> std::unique_ptr<command>
{
    return std::make_unique<update_layout_command>(*this);
}

update_layout_service::update_layout_service(command_history& cmds,
                                             const graph& g,
                                             weight_map edge_weight,
                                             layout_pointer& layout,
                                             const topology_pointer& space)
    : m_cmds(cmds),
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_space{space}
{
    assert(layout);
    assert(space);
}

void update_layout_service::operator()(descriptor desc)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_signal, std::move(desc), m_g, m_edge_weight, m_layout, m_space));
}

/***********************************************************
 * Update Topology Use Case                                *
 ***********************************************************/

class update_topology_command : public utility::command
{
public:
    using signal = update_topology_service::signal;
    using graph = update_topology_service::graph;
    using weight_map = update_topology_service::weight_map;
    using command_history = update_topology_service::command_history;
    using layout_pointer = update_topology_service::layout_pointer;
    using topology_pointer = update_topology_service::topology_pointer;
    using descriptor = update_topology_service::descriptor;
    using scale_type = update_topology_service::scale_type;

    update_topology_command(signal& s,
                            descriptor desc,
                            scale_type scale,
                            const graph& g,
                            weight_map edge_weight,
                            topology_pointer& space,
                            layout_pointer& l);

    virtual ~update_topology_command() override = default;

    virtual void execute() override;

    virtual void undo() override;
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<update_topology_command>(*this);
    }

private:
    void change_topology(const descriptor& desc, double topology_scale);

    signal& m_signal;

    descriptor m_desc;
    double m_scale;
    descriptor m_prev_desc;
    double m_prev_scale;

    const graph& m_g;
    weight_map m_edge_weight;
    topology_pointer& m_space;
    layout_pointer& m_layout;
};

update_topology_command::update_topology_command(signal& s,
                                                 descriptor desc,
                                                 scale_type scale,
                                                 const graph& g,
                                                 weight_map edge_weight,
                                                 topology_pointer& space,
                                                 layout_pointer& l)
    : m_signal{s},
      m_desc{std::move(desc)},
      m_scale{scale},
      m_prev_desc{space->desc()},
      m_prev_scale{space->scale()},
      m_g{g},
      m_edge_weight{edge_weight},
      m_space{space},
      m_layout{l}
{}

void update_topology_command::execute()
{
    if (m_desc != m_space->desc() or m_scale != m_space->scale())
        change_topology(m_desc, m_scale);
}

void update_topology_command::undo()
{
    if (m_prev_desc != m_space->desc() or m_prev_scale != m_space->scale())
        change_topology(m_prev_desc, m_prev_scale);
}

void update_topology_command::change_topology(const descriptor& desc,
                                              double scale)
{
    m_space = topology_factory::make_topology(desc, scale);
    m_layout = layout_factory::make_layout(
        m_layout->desc(), m_g, *m_space, m_edge_weight);

    BOOST_LOG_TRIVIAL(info)
        << "topology changed to: " << desc << " with scale: " << scale;
    BOOST_LOG_TRIVIAL(info) << "layout updated";

    m_signal(*m_layout, *m_space);
}

update_topology_service::update_topology_service(command_history& cmds,
                                                 const graph& g,
                                                 weight_map edge_weight,
                                                 layout_pointer& layout,
                                                 topology_pointer& topology)
    : m_cmds{cmds},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_topology{topology}
{
    assert(layout);
    assert(topology);
}

void update_topology_service::operator()(descriptor desc, scale_type scale)
{
    m_cmds.execute(std::make_unique<update_topology_command>(m_signal,
                                                             std::move(desc),
                                                             scale,
                                                             m_g,
                                                             m_edge_weight,
                                                             m_topology,
                                                             m_layout));
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
    m_layout = layout_factory::make_layout(
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
      m_layout{detail::layout_factory::make_layout(
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