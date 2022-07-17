// Contains the application specific usecases (interactors/services) of the
// layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "architecture/all.hpp"
#include "events.hpp"
#include "layout.hpp"
#include "layout_factory.hpp"
#include "topology_factory.hpp"
#include "utility/event_system.hpp"
#include "utility/undo_redo.hpp"

namespace layout
{

class update_layout_service
{
public:
    using graph = architecture::graph;
    using event_bus = utility::event_bus;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using type_name = layout_factory::type_name;

    update_layout_service(event_bus& pipeline,
                          command_history& cmds,
                          const graph& g,
                          const topology& space,
                          layout_pointer& l);

    void operator()(type_name type);

private:
    class update_layout_command : public command
    {
    public:
        update_layout_command(event_bus& pipeline,
                              type_name type,
                              const graph& g,
                              const topology& space,
                              layout_pointer& l);

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

        event_bus& m_pipeline;

        type_name m_type;
        type_name m_prev_type;

        const graph& m_g;
        const topology& m_space;
        layout_pointer& m_layout;
    };

    event_bus& m_pipeline;
    command_history& m_cmds;

    const graph& m_g;
    const topology& m_space;
    layout_pointer& m_layout;
};

class update_topology_service
{
public:
    using graph = architecture::graph;
    using event_bus = utility::event_bus;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using type_name = topology_factory::type_name;

    update_topology_service(event_bus& pipeline,
                            command_history& cmds,
                            const graph& g,
                            topology_pointer& space,
                            layout_pointer& l);

    void operator()(type_name type, double scale);

private:
    class update_topology_command : public command
    {
    public:
        update_topology_command(event_bus& pipeline,
                                type_name type,
                                double scale,
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

        event_bus& m_pipeline;

        type_name m_type;
        double m_scale;
        type_name m_prev_type;
        double m_prev_scale;

        const graph& m_g;
        topology_pointer& m_space;
        layout_pointer& m_layout;
    };

    event_bus& m_pipeline;

    command_history& m_cmds;

    const graph& m_g;
    topology_pointer& m_space;
    layout_pointer& m_layout;
};

} // namespace layout

#endif // LAYOUT_SERVICES_HPP
