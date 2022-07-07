// Contains the visualization/layout subsystem interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_MANAGER_HPP
#define LAYOUT_MANAGER_HPP

#include "architecture/graph.hpp"
#include "controller.hpp"
#include "presenter.hpp"
#include "service.hpp"
#include "view.hpp"

#include <memory>

namespace visualization
{

class layout_manager final
{
public:
    using graph = architecture::graph;

    layout_manager(const layout_manager&) = delete;
    layout_manager(layout_manager&&) = delete;

    auto operator=(const layout_manager&) -> layout_manager& = delete;
    auto operator=(layout_manager&&) -> layout_manager& = delete;

    static auto get() -> layout_manager&
    {
        static layout_manager singleton;
        return singleton;
    }

    void initialize(const graph& g, const Ogre::SceneManager& scene);
    void reset();

    auto service() const -> const auto&
    {
        assert(m_service);
        return *m_service;
    }

    auto service() -> auto&
    {
        assert(m_service);
        return *m_service;
    }

    auto view() const -> const auto&
    {
        assert(m_view);
        return *m_view;
    }

    auto view() -> auto&
    {
        assert(m_view);
        return *m_view;
    }

    auto controller() const -> const auto&
    {
        assert(m_controller);
        return *m_controller;
    }

    auto controller() -> auto&
    {
        assert(m_controller);
        return *m_controller;
    }

    auto presenter() const -> const auto&
    {
        assert(m_presenter);
        return *m_presenter;
    }

    auto presenter() -> auto&
    {
        assert(m_presenter);
        return *m_presenter;
    }

private:
    layout_manager() = default;
    ~layout_manager() = default;

    std::unique_ptr<layout_service> m_service;
    std::unique_ptr<layout_view> m_view;
    std::unique_ptr<layout_controller<>> m_controller;
    std::unique_ptr<layout_presenter<>> m_presenter;
};

} // namespace visualization

#endif // LAYOUT_MANAGER_HPP
