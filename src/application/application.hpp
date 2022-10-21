// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APPLICATION_HPP
#define APPLICATION_APPLICATION_HPP

#include "background_presenter.hpp"
#include "commands_presenter.hpp"
#include "graph_presenter.hpp"
#include "gui_presenter.hpp"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <memory>

namespace application
{

// Kick starts the application.
class application : public OgreBites::ApplicationContext
{
    using base = OgreBites::ApplicationContext;

public:
    using command_history_type = commands_presenter<>::command_history_type;
    using background_renderer_type = background_presenter<>::renderer_type;
    using graph_renderer_type = graph_presenter::graph_renderer_type;
    using gui_type = gui_presenter::gui_type;

    application(int argc, const char* argv[]);
    virtual ~application() override = default;

    auto setup() -> void override;
    auto shutdown() -> void override;

    auto go() -> void;

    inline auto get_graph_path() const -> std::string_view
    {
        return m_graph_path;
    }

    inline auto get_graph_interface() const -> graph_interface*
    {
        return m_graph_iface.get();
    }

    inline auto get_commands() const -> command_history_type*
    {
        return m_commands.get();
    }

    inline auto get_background_renderer() const -> background_renderer_type*
    {
        return m_background_renderer.get();
    }

    inline auto get_graph_renderer() const -> graph_renderer_type*
    {
        return m_graph_renderer.get();
    }

    // clang-format off
    inline auto get_gui() const -> gui_type*
    {
        return m_gui.get();
    }
    // clang-format on

    inline auto get_background_presenter() const -> background_presenter<>*
    {
        return m_background_presenter.get();
    }

    inline auto get_graph_presenter() const -> graph_presenter*
    {
        return m_graph_presenter.get();
    }

    inline auto get_commands_presenter() const -> commands_presenter<>*
    {
        return m_commands_presenter.get();
    }

    inline auto get_gui_presenter() const -> gui_presenter*
    {
        return m_gui_presenter.get();
    }

private:
    auto setup_graph() -> void;
    auto setup_commands() -> void;
    auto setup_background_renderer() -> void;
    auto setup_graph_renderer() -> void;
    auto setup_gui() -> void;
    auto setup_background_presenter() -> void;
    auto setup_graph_presenter() -> void;
    auto setup_commands_presenter() -> void;
    auto setup_gui_presenter() -> void;

    auto shutdown_gui_presenter() -> void;
    auto shutdown_commands_presenter() -> void;
    auto shutdown_graph_presenter() -> void;
    auto shutdown_background_presenter() -> void;
    auto shutdown_gui() -> void;
    auto shutdown_graph_renderer() -> void;
    auto shutdown_background_renderer() -> void;
    auto shutdown_commands() -> void;
    auto shutdown_graph() -> void;

    std::string_view m_graph_path;

    std::unique_ptr< graph_interface > m_graph_iface;
    std::unique_ptr< command_history_type > m_commands;
    std::unique_ptr< background_renderer_type > m_background_renderer;
    std::unique_ptr< graph_renderer_type > m_graph_renderer;
    std::unique_ptr< gui_type > m_gui;
    std::unique_ptr< background_presenter<> > m_background_presenter;
    std::unique_ptr< graph_presenter > m_graph_presenter;
    std::unique_ptr< commands_presenter<> > m_commands_presenter;
    std::unique_ptr< gui_presenter > m_gui_presenter;
};

} // namespace application

#endif // APPLICATION_APPLICATION_HPP
