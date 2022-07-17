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
    using signal = boost::signals2::signal<void(const layout&)>;

public:
    using graph = architecture::graph;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using type_name = layout_factory::type_name;
    using layout_listener = signal::slot_type;

    explicit update_layout_service(command_history& cmds);

    void operator()(type_name type,
                    const graph& g,
                    layout_pointer& layout,
                    const topology_pointer& space);

    void on_layout_response(const layout_listener& f) { m_signal.connect(f); }

private:
    class update_layout_command : public command
    {
    public:
        update_layout_command(signal& s,
                              type_name type,
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
        void change_layout(type_name type);

        signal& m_signal;

        type_name m_type;
        type_name m_prev_type;

        const graph& m_g;
        layout_pointer& m_layout;
        const topology_pointer& m_space;
    };

    signal m_signal;
    command_history& m_cmds;
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
    using type_name = topology_factory::type_name;
    using scale_type = topology_factory::scale_type;
    using layout_listener = signal::slot_type;

    explicit update_topology_service(command_history& cmds);

    void operator()(type_name type,
                    scale_type scale,
                    const graph& g,
                    layout_factory::pointer& layout,
                    topology_factory::pointer& topology);

    void on_layout_response(const layout_listener& f) { m_signal.connect(f); }

private:
    class update_topology_command : public command
    {
    public:
        update_topology_command(signal& s,
                                type_name type,
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
        void change_topology(type_name topology_type, double topology_scale);

        signal& m_signal;

        type_name m_type;
        double m_scale;
        type_name m_prev_type;
        double m_prev_scale;

        const graph& m_g;
        topology_pointer& m_space;
        layout_pointer& m_layout;
    };

    signal m_signal;
    command_history& m_cmds;
};

} // namespace layout

#endif // LAYOUT_SERVICES_HPP
