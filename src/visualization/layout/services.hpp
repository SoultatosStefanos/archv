// Contains the layout service of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICES_HPP
#define LAYOUT_SERVICES_HPP

#include "architecture/all.hpp"
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

class update_layout_service
{
public:
    using graph = architecture::graph;
    using event_bus = communication::event_bus;

    update_layout_service(event_bus& pipeline,
                          const graph& g,
                          std::unique_ptr<layout>& l,
                          std::string layout_type,
                          const topology* space);

    void update(const std::string& type);

protected:
    void change_layout(const std::string& type);

private:
    event_bus& m_pipeline;

    const graph& m_g;

    std::unique_ptr<layout>& m_layout;
    mutable std::string m_layout_type;
    const topology* m_space{nullptr};
};

class update_topology_service
{
public:
    using graph = architecture::graph;
    using event_bus = communication::event_bus;

    update_topology_service(event_bus& pipeline,
                            const graph& g,
                            topology& space,
                            std::string topology_type,
                            double scale)
        : m_pipeline{pipeline},
          m_g{g},
          m_space{space},
          m_space_type{std::move(topology_type)},
          m_space_scale{scale}
    {}

    void update(const std::string& type, double scale);

private:
    event_bus& m_pipeline;

    const graph& m_g;

    topology& m_space;
    mutable std::string m_space_type;
    mutable double m_space_scale;
};

} // namespace visualization::layout

#endif // LAYOUT_SERVICES_HPP
