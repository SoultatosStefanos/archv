// Contains the layout service of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SERVICE_HPP
#define LAYOUT_SERVICE_HPP

#include "architecture/all.hpp"
#include "layout.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace visualization
{

class layout_service
{
public:
    using graph = architecture::graph;
    using on_layout_changed = std::function<void(const layout&)>;

    explicit layout_service(const graph& g) : m_g{g} {}

    void initialize_layout(); // TODO tidy

    void update_layout(const std::string& type);
    void update_topology(const std::string& type, double scale);

    void add_layout_changed_listener(on_layout_changed f);

protected:
    void notify_layout_changed(const layout& l) const;

private:
    using layout_changed_listeners = std::vector<on_layout_changed>;

    const graph& m_g;

    layout_changed_listeners m_layout_changed_listeners;

    mutable std::string m_layout_type;
    mutable std::string m_topology_type;
    mutable double m_topology_scale;
    mutable topology m_topology;
    mutable std::unique_ptr<layout> m_layout;
};

} // namespace visualization

#endif // LAYOUT_SERVICE_HPP
