#ifndef LAYOUT_CONTROLLER_HPP
#define LAYOUT_CONTROLLER_HPP

#include "service.hpp"
#include "view.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace Layout
{

class Controller
{
public:
    Controller(View* view, Service* service) : m_view{view}, m_service{service}
    {
        assert(m_view);
        assert(m_service);

        m_view->add_layout_input_listener(
            [this](const auto& type) { update_layout(type); });

        m_view->add_topology_input_listener(
            [this](const auto& type, auto scale) {
                update_topology(type, scale);
            });
    }

    void update_layout(const std::string& type)
    {
        m_service->update_layout(type);
    }

    void update_topology(const std::string& type, double scale)
    {
        m_service->update_topology(type, scale);
    }

private:
    View* m_view{nullptr};
    Service* m_service{nullptr};
};

} // namespace Layout

#endif // LAYOUT_CONTROLLER_HPP
