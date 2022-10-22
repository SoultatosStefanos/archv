// Contains the application driver class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APPLICATION_HPP
#define APPLICATION_APPLICATION_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>
#include <memory>

/***********************************************************
 * Application                                             *
 ***********************************************************/

namespace application
{

// Initializes and hooks all of the subsystems.
// Kick starts the application.
class app : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
    using base = OgreBites::ApplicationContext;

public:
    app(int argc, const char* argv[]);
    virtual ~app() override = default;

    auto frameStarted(const Ogre::FrameEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

    auto setup() -> void override;
    auto shutdown() -> void override;

    auto go() -> void;

private:
    auto setup_configs() -> void;
    auto setup_graph_interface() -> void;
    auto setup_commands() -> void;
    auto setup_background_renderer() -> void;
    auto setup_graph_renderer() -> void;
    auto setup_gui() -> void;

    auto prepare_weights_editor() -> void;
    auto prepare_layout_editor() -> void;
    auto prepare_scaling_editor() -> void;
    auto prepare_background_configurator() -> void;
    auto prepare_graph_configurator() -> void;
    auto prepare_gui_configurator() -> void;
    auto prepare_gui() -> void;
};

} // namespace application

#endif // APPLICATION_APPLICATION_HPP
