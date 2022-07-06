
#ifndef LAYOUT_SERVICE_HPP
#define LAYOUT_SERVICE_HPP

#include "layout.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Layout
{

class Service
{
public:
    using LayoutObserver = std::function<void(const Layout&)>;

    Service() = default;
    Service(const Service&) = default;
    Service(Service&&) = default;

    virtual ~Service() = default;

    auto operator=(const Service&) -> Service& = default;
    auto operator=(Service&&) -> Service& = default;

    virtual void update_layout(const std::string& type) = 0;
    virtual void update_topology(const std::string& type, double scale) = 0;

    void add_layout_listener(LayoutObserver f)
    {
        m_obs.push_back(std::move(f));
    }

    virtual auto clone() const -> std::unique_ptr<Service> = 0;

protected:
    void notify_layout_changed(const Layout& l) const
    {
        for (const auto& f : m_obs)
            f(l);
    }

private:
    using Observers = std::vector<LayoutObserver>;

    Observers m_obs;
};

} // namespace Layout

#endif // LAYOUT_SERVICE_HPP
