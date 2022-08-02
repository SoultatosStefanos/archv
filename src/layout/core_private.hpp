// Contains a private core submodule.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_PRIVATE_HPP
#define LAYOUT_CORE_PRIVATE_HPP

#include "architecture/graph.hpp"
#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "topology.hpp"
#include "utility/undo_redo.hpp"

#include <boost/log/trivial.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>

namespace layout::detail
{

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

template <typename Graph>
class layout_factory final
{
public:
    using graph = Graph;
    using pointer = std::unique_ptr<layout<graph>>;
    using descriptor = typename layout<graph>::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    template <typename WeightMap>
    static auto make_layout(const descriptor& desc,
                            const graph& g,
                            const topology& space,
                            WeightMap edge_weight) -> pointer
    {
        if (desc == layout_traits<gursoy_atun_layout<graph>>::desc())
        {
            return std::make_unique<gursoy_atun_layout<graph>>(
                g, space, edge_weight);
        }
        else
        {
            BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
            assert(false);
            return nullptr;
        }
    }

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using scale_type = topology::scale_type;
    using descriptor = topology::descriptor;

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto make_topology(const descriptor& desc, scale_type scale)
        -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

/***********************************************************
 * Update Layout Use Case                                  *
 ***********************************************************/

template <typename Graph, typename WeightMap>
class update_layout_command;

template <typename Graph, typename WeightMap>
class update_layout_service
{
    using signal = boost::signals2::signal<void(const layout<Graph>&)>;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using command_history = utility::command_history;
    using layout_pointer = typename layout_factory<graph>::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = typename layout_factory<graph>::descriptor;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    update_layout_service(command_history& cmds,
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

    void operator()(descriptor desc)
    {
        m_cmds.execute(
            std::make_unique<update_layout_command<graph, weight_map>>(
                m_signal,
                std::move(desc),
                m_g,
                m_edge_weight,
                m_layout,
                m_space));
    }

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class update_layout_command<graph, weight_map>;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

template <typename Graph, typename WeightMap>
class update_layout_command : public utility::command
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using signal = typename update_layout_service<graph, weight_map>::signal;
    using command_history = utility::command_history;
    using layout_pointer = typename layout_factory<graph>::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = typename layout_factory<graph>::descriptor;

    update_layout_command(signal& s,
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

    virtual ~update_layout_command() override = default;

    virtual void execute() override
    {
        if (m_desc != m_layout->desc())
            change_layout(m_desc);
    }

    virtual void undo() override
    {
        if (m_prev_desc != m_layout->desc())
            change_layout(m_prev_desc);
    }

    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<update_layout_command>(*this);
    }

private:
    void change_layout(const descriptor& desc)
    {
        m_layout = layout_factory<graph>::make_layout(
            desc, m_g, *m_space, m_edge_weight);

        BOOST_LOG_TRIVIAL(info) << "layout changed to: " << desc;

        m_signal(*m_layout);
    }

    signal& m_signal;

    descriptor m_desc;
    descriptor m_prev_desc;

    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

/***********************************************************
 * Update Topology Use Case                                *
 ***********************************************************/

template <typename Graph, typename WeightMap>
class update_topology_command;

template <typename Graph, typename WeightMap>
class update_topology_service
{
    using signal =
        boost::signals2::signal<void(const layout<Graph>&, const topology&)>;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using command_history = utility::command_history;
    using layout_pointer = typename layout_factory<graph>::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    update_topology_service(command_history& cmds,
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

    void operator()(descriptor desc, scale_type scale)
    {
        m_cmds.execute(
            std::make_unique<update_topology_command<graph, weight_map>>(
                m_signal,
                std::move(desc),
                scale,
                m_g,
                m_edge_weight,
                m_topology,
                m_layout));
    }

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class update_topology_command<Graph, WeightMap>;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

template <typename Graph, typename WeightMap>
class update_topology_command : public utility::command
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using signal = typename update_topology_service<graph, weight_map>::signal;
    using command_history = utility::command_history;
    using layout_pointer = typename layout_factory<graph>::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;

    update_topology_command(signal& s,
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

    virtual ~update_topology_command() override = default;

    virtual void execute() override
    {
        if (m_desc != m_space->desc() or m_scale != m_space->scale())
            change_topology(m_desc, m_scale);
    }
    virtual void undo() override
    {
        if (m_prev_desc != m_space->desc() or m_prev_scale != m_space->scale())
            change_topology(m_prev_desc, m_prev_scale);
    }
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<update_topology_command>(*this);
    }

private:
    void change_topology(const descriptor& desc, double scale)
    {
        m_space = topology_factory::make_topology(desc, scale);
        m_layout = layout_factory<graph>::make_layout(
            m_layout->desc(), m_g, *m_space, m_edge_weight);

        BOOST_LOG_TRIVIAL(info)
            << "topology changed to: " << desc << " with scale: " << scale;
        BOOST_LOG_TRIVIAL(info) << "layout updated";

        m_signal(*m_layout, *m_space);
    }

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

/***********************************************************
 * Revert to Defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_command;

class revert_to_defaults_service
{
    using signal = boost::signals2::signal<void(
        const layout<architecture::graph>&, const topology&)>;

public:
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using command_history = utility::command_history;
    using layout_pointer = typename layout_factory<graph>::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = typename layout_factory<graph>::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    revert_to_defaults_service(command_history& cmds,
                               const graph& g,
                               weight_map edge_weight,
                               layout_pointer& layout,
                               topology_pointer& space);

    void operator()();

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class revert_to_defaults_command;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_descriptor m_layout_desc;
    topology_descriptor m_topology_desc;
    topology_scale m_topology_scale;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

} // namespace layout::detail

#endif // LAYOUT_CORE_PRIVATE_HPP
