// Contains the layout service of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "architecture/all.hpp"
#include "command/command.hpp"
#include "events.hpp"
#include "layout.hpp"
#include "visualization/communication/all.hpp"

#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace visualization::layout
{

class update_layout_service : public command::command
{
public:
    using graph = architecture::graph;
    using event_bus = communication::event_bus;
    using layout_factory = std::function<std::unique_ptr<layout>(
        const std::string&, const graph&, const topology&)>;

    update_layout_service(event_bus& pipeline,
                          layout_request_event e,
                          const graph& g,
                          const topology& space,
                          std::unique_ptr<layout>& l,
                          layout_factory make_layout);

    virtual ~update_layout_service() override = default;

    virtual void execute() override;

    virtual void undo() override;
    virtual void redo() override;

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<update_layout_service>(*this);
    }

private:
    event_bus& m_pipeline;

    layout_request_event m_request;

    const graph& m_g;
    const topology& m_space;
    std::unique_ptr<layout>& m_layout;

    layout_factory m_make_layout;
};

class update_topology_service : public command::command
{
public:
    using graph = architecture::graph;
    using event_bus = communication::event_bus;
    using topology_factory =
        std::function<topology(const std::string&, double)>;
    using layout_factory = std::function<std::unique_ptr<layout>(
        const std::string&, const graph&, const topology&)>;

    update_topology_service(event_bus& pipeline,
                            topology_request_event e,
                            const graph& g,
                            topology& space,
                            std::unique_ptr<layout>& l,
                            topology_factory make_topology,
                            layout_factory make_layout);

    virtual ~update_topology_service() override = default;

    virtual void execute() override;

    virtual void undo() override;
    virtual void redo() override;

    virtual auto clone() const -> std::unique_ptr<command> override
    {
        return std::make_unique<update_topology_service>(*this);
    }

private:
    event_bus& m_pipeline;

    topology_request_event m_request;

    const graph& m_g;
    topology& m_space;
    std::unique_ptr<layout>& m_layout;

    topology_factory m_make_topology;
    layout_factory m_make_layout;
};

} // namespace visualization::layout

#endif // LAYOUT_SERVICES_HPP
