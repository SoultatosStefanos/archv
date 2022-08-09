// Contains a private core submodule.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_PRIVATE_HPP
#define LAYOUT_CORE_PRIVATE_HPP

#include "gursoy_atun_layout.hpp"
#include "layout.hpp"
#include "topology.hpp"
#include "undo_redo/all.hpp"

#include <boost/log/trivial.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>

namespace layout::detail
{

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

template < typename Graph >
class layout_factory final
{
public:
    using graph = Graph;
    using pointer = std::unique_ptr< layout< graph > >;
    using descriptor = typename layout< graph >::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    template < typename WeightMap >
    static auto make_layout(
        const descriptor& desc,
        const graph& g,
        const topology& space,
        WeightMap edge_weight) -> pointer
    {

        if (desc == layout_traits< gursoy_atun_layout< graph > >::desc())
        {
            return std::make_unique< gursoy_atun_layout< graph > >(
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
    using pointer = std::unique_ptr< topology >;
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

template < typename Graph, typename WeightMap >
class update_layout_command;

template < typename Graph, typename WeightMap >
class update_layout_service
{
    using signal = boost::signals2::signal< void(const layout< Graph >&) >;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = typename layout_factory< graph >::descriptor;
    using slot_type = typename signal::slot_type;
    using connection = boost::signals2::connection;

    update_layout_service(
        command_history& cmds,
        const graph& g,
        weight_map edge_weight,
        layout_pointer& layout,
        const topology_pointer& space)
    : m_cmds(cmds)
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_layout { layout }
    , m_space { space }
    {
        assert(layout);
        assert(space);
    }

    void operator()(descriptor new_desc)
    {
        m_cmds.execute(
            std::make_unique< update_layout_command< graph, weight_map > >(
                m_signal,
                std::move(new_desc),
                m_g,
                m_edge_weight,
                m_layout,
                m_space));
    }

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class update_layout_command< graph, weight_map >;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

template < typename Graph, typename WeightMap >
class update_layout_command : public undo_redo::command
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using signal = typename update_layout_service< graph, weight_map >::signal;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = typename layout_factory< graph >::descriptor;

    update_layout_command(
        signal& s,
        descriptor new_desc,
        const graph& g,
        weight_map edge_weight,
        layout_pointer& layout,
        const topology_pointer& space)
    : m_signal { s }
    , m_new_desc { new_desc }
    , m_prev_desc { layout->desc() }
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_layout { layout }
    , m_space { space }
    {
    }

    virtual ~update_layout_command() override = default;

    virtual void execute() override
    {
        if (m_new_desc != m_layout->desc())
            change_layout(m_new_desc);
    }

    virtual void undo() override
    {
        if (m_prev_desc != m_layout->desc())
            change_layout(m_prev_desc);
    }

    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr< command > override
    {
        return std::make_unique< update_layout_command >(*this);
    }

private:
    void change_layout(const descriptor& desc)
    {
        m_layout = layout_factory< graph >::make_layout(
            desc, m_g, *m_space, m_edge_weight);

        BOOST_LOG_TRIVIAL(info) << "layout changed to: " << desc;

        m_signal(*m_layout);
    }

    signal& m_signal;

    descriptor m_new_desc;
    descriptor m_prev_desc;

    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

/***********************************************************
 * Update Topology Use Case                                *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class update_topology_command;

template < typename Graph, typename WeightMap >
class update_topology_service
{
    using signal = boost::signals2::signal< void(
        const layout< Graph >&, const topology&) >;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;
    using slot_type = typename signal::slot_type;
    using connection = boost::signals2::connection;

    update_topology_service(
        command_history& cmds,
        const graph& g,
        weight_map edge_weight,
        layout_pointer& layout,
        topology_pointer& topology)
    : m_cmds { cmds }
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_layout { layout }
    , m_topology { topology }
    {
        assert(layout);
        assert(topology);
    }

    void operator()(descriptor new_desc, scale_type new_scale)
    {
        m_cmds.execute(
            std::make_unique< update_topology_command< graph, weight_map > >(
                m_signal,
                std::move(new_desc),
                new_scale,
                m_g,
                m_edge_weight,
                m_topology,
                m_layout));
    }

    connection connect(const slot_type& f) { return m_signal.connect(f); }

    friend class update_topology_command< Graph, WeightMap >;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

template < typename Graph, typename WeightMap >
class update_topology_command : public undo_redo::command
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using signal =
        typename update_topology_service< graph, weight_map >::signal;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;

    update_topology_command(
        signal& s,
        descriptor new_desc,
        scale_type new_scale,
        const graph& g,
        weight_map edge_weight,
        topology_pointer& space,
        layout_pointer& l)
    : m_signal { s }
    , m_new_desc { std::move(new_desc) }
    , m_new_scale { new_scale }
    , m_prev_desc { space->desc() }
    , m_prev_scale { space->scale() }
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_space { space }
    , m_layout { l }
    {
    }

    virtual ~update_topology_command() override = default;

    virtual void execute() override
    {
        if (m_new_desc != m_space->desc() or m_new_scale != m_space->scale())
            change_topology(m_new_desc, m_new_scale);
    }

    virtual void undo() override
    {
        if (m_prev_desc != m_space->desc() or m_prev_scale != m_space->scale())
            change_topology(m_prev_desc, m_prev_scale);
    }

    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr< command > override
    {
        return std::make_unique< update_topology_command >(*this);
    }

private:
    void change_topology(const descriptor& desc, double scale)
    {
        m_space = topology_factory::make_topology(desc, scale);
        m_layout = layout_factory< graph >::make_layout(
            m_layout->desc(), m_g, *m_space, m_edge_weight);

        BOOST_LOG_TRIVIAL(info)
            << "topology changed to: " << desc << " with scale: " << scale;
        BOOST_LOG_TRIVIAL(info) << "layout updated";

        m_signal(*m_layout, *m_space);
    }

    signal& m_signal;

    descriptor m_new_desc;
    double m_new_scale;
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

template < typename Graph, typename WeightMap >
class revert_to_defaults_command;

template < typename Graph, typename WeightMap >
class revert_to_defaults_service
{
    using signal = boost::signals2::signal< void(
        const layout< Graph >&, const topology&) >;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = typename layout_factory< graph >::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using slot_type = typename signal::slot_type;
    using connection = boost::signals2::connection;

    revert_to_defaults_service(
        command_history& cmds,
        const graph& g,
        weight_map edge_weight,
        layout_pointer& layout,
        topology_pointer& space)
    : m_cmds { cmds }
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_initial_layout_desc { layout->desc() }
    , m_initial_topology_desc { space->desc() }
    , m_initial_topology_scale { space->scale() }
    , m_layout { layout }
    , m_topology { space }
    {
    }

    void operator()()
    {
        m_cmds.execute(
            std::make_unique< revert_to_defaults_command< graph, weight_map > >(
                m_signal,
                m_g,
                m_edge_weight,
                m_initial_layout_desc,
                m_initial_topology_desc,
                m_initial_topology_scale,
                m_layout,
                m_topology));
    }

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class revert_to_defaults_command< graph, weight_map >;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_descriptor m_initial_layout_desc;
    topology_descriptor m_initial_topology_desc;
    topology_scale m_initial_topology_scale;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

template < typename Graph, typename WeightMap >
class revert_to_defaults_command : public undo_redo::command
{
public:
    using graph = Graph;
    using weight_map = WeightMap;
    using signal =
        typename revert_to_defaults_service< Graph, WeightMap >::signal;
    using command_history = undo_redo::command_history;
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = typename layout_factory< graph >::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;

    revert_to_defaults_command(
        signal& s,
        const graph& g,
        weight_map edge_weight,
        layout_descriptor initial_layout_desc,
        topology_descriptor initial_topology_desc,
        topology_scale initial_topology_scale,
        layout_pointer& layout,
        topology_pointer& space)
    : m_signal { s }
    , m_g { g }
    , m_edge_weight { edge_weight }
    , m_initial_layout_desc { initial_layout_desc }
    , m_prev_layout_desc { layout->desc() }
    , m_initial_topology_desc { initial_topology_desc }
    , m_prev_topology_desc { space->desc() }
    , m_initial_topology_scale { initial_topology_scale }
    , m_prev_topology_scale { space->scale() }
    , m_layout { layout }
    , m_topology { space }
    {
    }

    virtual ~revert_to_defaults_command() override = default;

    virtual void execute() override
    {
        if (m_initial_layout_desc != m_layout->desc()
            or m_initial_topology_desc != m_topology->desc()
            or m_initial_topology_scale != m_topology->scale())
            change_layout(
                m_initial_layout_desc,
                m_initial_topology_desc,
                m_initial_topology_scale);
    }

    virtual void undo() override
    {
        if (m_prev_layout_desc != m_layout->desc()
            or m_prev_topology_desc != m_topology->desc()
            or m_prev_topology_scale != m_topology->scale())
            change_layout(
                m_prev_layout_desc,
                m_prev_topology_desc,
                m_prev_topology_scale);
    }

    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr< command > override
    {
        return std::make_unique< revert_to_defaults_command >(*this);
    }

private:
    void change_layout(
        const layout_descriptor& layout_desc,
        const topology_descriptor& topology_desc,
        topology_scale topology_scale)
    {
        m_topology
            = topology_factory::make_topology(topology_desc, topology_scale);
        m_layout = layout_factory< graph >::make_layout(
            layout_desc, m_g, *m_topology, m_edge_weight);

        BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_desc
                                << " with scale: " << topology_scale;
        BOOST_LOG_TRIVIAL(info) << "layout changed to: " << layout_desc;

        m_signal(*m_layout, *m_topology);
    }

    signal& m_signal;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_descriptor m_initial_layout_desc;
    layout_descriptor m_prev_layout_desc;
    topology_descriptor m_initial_topology_desc;
    topology_descriptor m_prev_topology_desc;
    topology_scale m_initial_topology_scale;
    topology_scale m_prev_topology_scale;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

} // namespace layout::detail

#endif // LAYOUT_CORE_PRIVATE_HPP
