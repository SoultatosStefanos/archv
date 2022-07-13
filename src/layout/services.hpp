// Contains the layout service of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "architecture/all.hpp"
#include "events.hpp"
#include "layout.hpp"
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

    update_layout_service(event_bus& pipeline,
                          command_history& cmds,
                          const graph& g,
                          const topology& space,
                          std::unique_ptr<layout>& l);

    void operator()(const layout_request_event& e);

private:
    class update_layout_command : public command
    {
    public:
        update_layout_command(event_bus& pipeline,
                              layout_request_event e,
                              const graph& g,
                              const topology& space,
                              std::unique_ptr<layout>& l);

        virtual ~update_layout_command() override = default;

        virtual void execute() override;

        virtual void undo() override;
        virtual void redo() override { execute(); }

        virtual auto clone() const -> std::unique_ptr<command> override
        {
            return std::make_unique<update_layout_command>(*this);
        }

    private:
        void change_layout(const std::string& type);

        event_bus& m_pipeline;

        layout_request_event m_request;

        const graph& m_g;
        const topology& m_space;
        std::unique_ptr<layout>& m_layout;
    };

    event_bus& m_pipeline;
    command_history& m_cmds;

    const graph& m_g;
    const topology& m_space;
    std::unique_ptr<layout>& m_layout;
};

class update_topology_service
{
public:
    using graph = architecture::graph;
    using event_bus = utility::event_bus;
    using command_history = utility::command_history;
    using command = utility::command;

    update_topology_service(event_bus& pipeline,
                            command_history& cmds,
                            const graph& g,
                            topology& space,
                            std::unique_ptr<layout>& l);

    void operator()(const topology_request_event& e);

private:
    class update_topology_command : public command
    {
    public:
        update_topology_command(event_bus& pipeline,
                                topology_request_event e,
                                const graph& g,
                                topology& space,
                                std::unique_ptr<layout>& l);

        virtual ~update_topology_command() override = default;

        virtual void execute() override;

        virtual void undo() override;
        virtual void redo() override { execute(); }

        virtual auto clone() const -> std::unique_ptr<command> override
        {
            return std::make_unique<update_topology_command>(*this);
        }

    private:
        void change_topology(const std::string& topology_type,
                             double topology_scale,
                             const std::string& layout_type);

        event_bus& m_pipeline;

        topology_request_event m_request;

        const graph& m_g;
        topology& m_space;
        std::unique_ptr<layout>& m_layout;
    };

    event_bus& m_pipeline;

    command_history& m_cmds;

    const graph& m_g;
    topology& m_space;
    std::unique_ptr<layout>& m_layout;
};

} // namespace layout

#endif // LAYOUT_SERVICES_HPP
