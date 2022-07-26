// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include "dependencies/all.hpp"
#include "features/layout/all.hpp"
#include "utility/all.hpp"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Ogre.h>
#include <memory>

// The heart of the application.
class app : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
public:
    using graph = dependencies::graph;

    explicit app(const graph& g);
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

    virtual auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    using command_history = utility::command_history;

    void initialize_layout();

    const graph& m_g;

    Ogre::SceneManager* m_scene{nullptr};
    OgreBites::CameraMan* m_cameraman{nullptr};

    command_history m_cmds;
};

#endif // APP_HPP
