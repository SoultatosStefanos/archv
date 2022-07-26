// Contains the application specific usecases of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "dependencies/graph.hpp"
#include "factories.hpp"
#include "layout.hpp"
#include "utility/undo_redo.hpp"

#include <boost/signals2/signal.hpp>

namespace features::layout
{

/***********************************************************
 * Update Layout Use Case                                  *
 ***********************************************************/

class update_layout_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = layout_factory::descriptor;
    using slot_type = signal::slot_type;

    update_layout_service(command_history& cmds,
                          const graph& g,
                          weight_map edge_weight,
                          layout_pointer& layout,
                          const topology_pointer& space);

    void execute(descriptor desc);

    void connect(const slot_type& f) { m_signal.connect(f); }

private:
    class update_layout_command : public command
    {
    public:
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

        virtual auto clone() const -> std::unique_ptr<command> override
        {
            return std::make_unique<update_layout_command>(*this);
        }

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

    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

/***********************************************************
 * Update Topology Use Case                                *
 ***********************************************************/

class update_topology_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;
    using slot_type = signal::slot_type;

    update_topology_service(command_history& cmds,
                            const graph& g,
                            weight_map edge_weight,
                            layout_pointer& layout,
                            topology_pointer& topology);

    void execute(descriptor desc, scale_type scale);

    void connect(const slot_type& f) { m_signal.connect(f); }

private:
    class update_topology_command : public command
    {
    public:
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

    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

/***********************************************************
 * Revert to Defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = layout_factory::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using slot_type = signal::slot_type;

    revert_to_defaults_service(command_history& cmds,
                               const graph& g,
                               weight_map edge_weight,
                               layout_descriptor layout_desc,
                               topology_descriptor topology_desc,
                               topology_scale topology_scale,
                               layout_pointer& layout,
                               topology_pointer& space);

    void execute();

    void connect(const slot_type& f) { m_signal.connect(f); }

private:
    class revert_to_defaults_command : public command
    {
    public:
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

} // namespace features::layout

#endif // LAYOUT_SERVICES_HPP
