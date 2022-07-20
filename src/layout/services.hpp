// Contains the application specific usecases (interactors/services) of the
// layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "architecture/graph.hpp"
#include "factories.hpp"
#include "layout.hpp"
#include "utility/undo_redo.hpp"

#include <boost/signals2/signal.hpp>

namespace layout
{

class update_layout_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = architecture::graph;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = layout_factory::descriptor;
    using layout_listener = signal::slot_type;

    update_layout_service(command_history& cmds,
                          const graph& g,
                          layout_pointer& layout,
                          const topology_pointer& space);

    void execute(descriptor desc);

    void on_layout_response(const layout_listener& f) { m_signal.connect(f); }

private:
    class update_layout_command : public command
    {
    public:
        update_layout_command(signal& s,
                              descriptor desc,
                              const graph& g,
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
        layout_pointer& m_layout;
        const topology_pointer& m_space;
    };

    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

class update_topology_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = architecture::graph;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;
    using layout_listener = signal::slot_type;

    update_topology_service(command_history& cmds,
                            const graph& g,
                            layout_factory::pointer& layout,
                            topology_factory::pointer& topology);

    void execute(descriptor desc, scale_type scale);

    void on_layout_response(const layout_listener& f) { m_signal.connect(f); }

private:
    class update_topology_command : public command
    {
    public:
        update_topology_command(signal& s,
                                descriptor desc,
                                scale_type scale,
                                const graph& g,
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
        topology_pointer& m_space;
        layout_pointer& m_layout;
    };

    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    layout_factory::pointer& m_layout;
    topology_factory::pointer& m_topology;
};

} // namespace layout

#endif // LAYOUT_SERVICES_HPP
