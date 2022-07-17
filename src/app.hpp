// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include "architecture/all.hpp"
#include "layout/all.hpp"
#include "utility/all.hpp"

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>
#include <OgreInput.h>
#include <memory>

// The heart of the application.
class app : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
public:
    using graph = architecture::graph;

    explicit app(const graph& g);
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

    virtual auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    using command_history = utility::command_history;
    using layout_presenter = layout::presenter<layout::view,
                                               layout::update_layout_service,
                                               layout::update_topology_service>;

    const graph& m_g;

    Ogre::SceneManager* m_scene{nullptr};
    OgreBites::CameraMan* m_cameraman{nullptr};

    command_history m_cmds;

    std::unique_ptr<layout_presenter> m_layout_core;
};

#endif // APP_HPP
