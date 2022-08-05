
#ifndef APP_HPP
#define APP_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <memory>

/***********************************************************
 * Subsystems                                              *
 ***********************************************************/

#include "architecture/allfwd.hpp"
#include "config/allfwd.hpp"
#include "dependencies/allfwd.hpp"
#include "layout/allfwd.hpp"
#include "undo_redo/allfwd.hpp"
#include "view/allfwd.hpp"

/***********************************************************
 * Application                                             *
 ***********************************************************/

// Initializes and hooks all of the subsystems.
// Kick starts the application.
class app : public OgreBites::ApplicationContext
{
public:
    explicit app(const std::string& name = "ARCHV");
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

protected:
    auto view_manager() const -> const view::core&;
    auto view_manager() -> view::core&;

    auto cmd_history() const -> const undo_redo::command_history&;
    auto cmd_history() -> undo_redo::command_history&;

    auto arch_manager() const -> const architecture::core&;
    auto arch_manager() -> architecture::core&;

    auto deps_manager() const -> const dependencies::core&;
    auto deps_manager() -> dependencies::core&;

    auto layout_manager() const -> const layout::core&;
    auto layout_manager() -> layout::core&;

private:
    void setup_view();
    void setup_undo_redo();
    void setup_architecture();
    void setup_dependencies();
    void setup_layout();

    void shutdown_layout();
    void shutdown_dependencies();
    void shutdown_architecture();
    void shutdown_undo_redo();
    void shutdown_view();

    void connect_layout_with_view();
    void connect_dependencies_with_view();

    void disconnect_layout_with_view();
    void disconnect_dependencies_with_view();

    std::unique_ptr< view::core > m_view_manager;
    std::unique_ptr< undo_redo::command_history > m_cmds;
    std::unique_ptr< architecture::core > m_arch_manager;
    std::unique_ptr< dependencies::core > m_deps_manager;
    std::unique_ptr< layout::core > m_layout_manager;
};

#endif // APP_HPP
